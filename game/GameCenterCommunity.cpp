/*
#include "GameCenterCommunity.h"
#include <GameKit/GameKit.h>
#include <Utils/Utils/Path.h>
#include "Log.h"
#include "IGameCenterViewProxy.h"
#include "Environment.h"

GameCenterCommunity::GameCenterCommunity(IGameCenterViewProxy *gameCenterViewProxy)
{
	this->gameCenterViewProxy = gameCenterViewProxy;
	
	errAchievements = nil;
	srvAchievements = nil;
	
	ladMainPoints = 0;

	dataAccessLock = [[NSLock alloc] init];
}

void GameCenterCommunity::SetStoredAchievesFile(const std::string &storedAchievesFile)
{
	this->storedAchievesFile = storedAchievesFile;	
}

void GameCenterCommunity::LoadAchievementsFromServer()
{
	Log::LogT("Loading achievements from server");
	
	[dataAccessLock lock];
	if (srvAchievements == nil)
		srvAchievements = [[NSMutableDictionary alloc] init];
	
	if (srvAchievements == nil) // error
	{
		[dataAccessLock unlock];
		return;
	}
	
	[srvAchievements removeAllObjects];
	[dataAccessLock unlock];
	
	[GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements,
														   NSError *error) {
		if (error != nil)
		{
			Log::LogE("Error occured during loading achievements %s", [[error localizedDescription] UTF8String]);
		}
		
		if (achievements != nil && [achievements count] > 0)
		{
			Log::LogT("Server returned %d achievements", [achievements count]);

			[dataAccessLock lock];
			for (GKAchievement *ach in achievements)
			{
				[srvAchievements setObject:ach forKey:ach.identifier];
				
				// remove from offline achievements, cos we dont need that anymore
				if (errAchievements != nil)
					[errAchievements removeObjectForKey:ach.identifier];
			}
			[dataAccessLock unlock];
			
			// update offline achievements list
			if (IsAuthenticated())
				StoreOfflineAchievements();
		}
		
		// send offline achievements to server
		ReportOfflineAchievements();
	}];

	Log::LogT("Loading achievements from server finished");
}

void GameCenterCommunity::LoadPointsFromServer()
{
	Log::LogT("Loading poits from server");
	
	if (!IsAuthenticated())
	{
		Log::LogW("Player is not authenticated");
		return;
	}

	[dataAccessLock lock];
	NSMutableArray *playerIdArr = [[NSMutableArray alloc] init];
	[playerIdArr addObject:[NSString stringWithUTF8String:playerId.c_str()]];
	
	GKLeaderboard *ladder = [[GKLeaderboard alloc] initWithPlayerIDs:playerIdArr];
	if (ladder == nil)
	{
		Log::LogE("Couldn't create GKLeadderboard object");
		assert(false);
	}
	[dataAccessLock unlock];
	
	ladder.playerScope = GKLeaderboardPlayerScopeGlobal;
	ladder.timeScope = GKLeaderboardTimeScopeAllTime;
	ladder.range = NSMakeRange(1,1);
	
	[ladder loadScoresWithCompletionHandler: ^(NSArray *scores,
											   NSError *error) {
		if (error != nil)
		{
			Log::LogE("Error occured during loading scores %s", [[error localizedDescription] UTF8String]);
		}
		else if (scores != nil && [scores count] > 0)
		{
			assert([scores count] == 1);
			
			Log::LogT("Server returned %d scores elements", [scores count]);
			
			[dataAccessLock lock];
			GKScore *score = [scores objectAtIndex:0];
			Log::LogT("Server returned scores: %d", score.value);
			ladMainPoints = score.value;
			[dataAccessLock unlock];
			
			ObsCast(IGameCommunityObserver, this)->NotifyObservers(&IGameCommunityObserver::PointsLoaded, score.value);
		}
		else if (scores == nil)
		{
			Log::LogT("Server returned null scores. It is considered as 0");
			ObsCast(IGameCommunityObserver, this)->NotifyObservers(&IGameCommunityObserver::PointsLoaded, 0);
		}
	}];
	
	[ladder release];
	Log::LogT("Loading achievements from server finished");
}

void GameCenterCommunity::LoadOfflineAchievements()
{
	Log::LogT("Loading offline achievements");
	
	assert(!storedAchievesFile.empty());

	[dataAccessLock lock];
	
	if (errAchievements != nil)
	{
		[errAchievements removeAllObjects];
		[errAchievements release];
		errAchievements = nil;
	}
	
	if (Path::IsFileExists(storedAchievesFile.c_str()))
	{
		Log::LogT("Found offline achievements file");
		
		NSString *nsStoredAchievesFile = [NSString stringWithUTF8String:storedAchievesFile.c_str()];
		//errAchievements = [[NSMutableDictionary alloc] initWithDictionary:[NSKeyedUnarchiver unarchiveObjectWithFile:nsStoredAchievesFile]];
		errAchievements = [[NSKeyedUnarchiver unarchiveObjectWithFile:nsStoredAchievesFile] retain];
		
		if (errAchievements == nil)
		{
			[dataAccessLock unlock];
			assert(false && "Could'n unarchive achieviements file");
			Log::LogE("Could'n unarchive achieviements file");
			return;
		}
				
		Log::LogT("Loaded %d offline achievements", [errAchievements count]);
	}
	
	[dataAccessLock unlock];
}

void GameCenterCommunity::ReportOfflineAchievements()
{
	Log::LogT("Sending offline achievements to server");

	[dataAccessLock lock];
	if (errAchievements == nil || [errAchievements count] == 0)
	{
		Log::LogT("No offline achievements");
		[dataAccessLock unlock];
		return;
	}
	
	NSMutableDictionary *copyOfErrAch = [errAchievements copy];
	if (copyOfErrAch == nil)
	{
		Log::LogE("Couldn't copy offline achievements");
		[dataAccessLock unlock];
		return;
	}
	
	reportingAchievesLeft = [copyOfErrAch count];
	[dataAccessLock unlock];
	
	for (NSString *achKey in copyOfErrAch)
	{
		GKAchievement *ach = [copyOfErrAch objectForKey:achKey];
		
		if (Environment::GetInstance()->GetIOSVersion() >= "5.0")
			ach.showsCompletionBanner = FALSE;

		[ach reportAchievementWithCompletionHandler:^(NSError *error) {
			if (error == nil)
			{
				Log::LogT("Achievement sended to server", [[error localizedDescription] UTF8String]);
				[dataAccessLock lock];
				[errAchievements removeObjectForKey:ach.identifier];
				[dataAccessLock unlock];
			}
			else
			{
				Log::LogE("Couldn't send achievement to serwer %s", [[error localizedDescription] UTF8String]);
			}
			
			[dataAccessLock lock];
			reportingAchievesLeft--;
			[dataAccessLock unlock];
			if (reportingAchievesLeft <= 0)
			{
				if (IsAuthenticated())
					StoreOfflineAchievements();
			}
		}];
	}
	
	Log::LogT("Finished sending achievements to server");
}

void GameCenterCommunity::StoreOfflineAchievements()
{
	assert(!storedAchievesFile.empty());
	Log::LogT("Storing offline achievements to file %s", storedAchievesFile.c_str());
	
	[dataAccessLock lock];
	if (errAchievements != nil)
		[NSKeyedArchiver archiveRootObject:errAchievements toFile:[NSString stringWithUTF8String:storedAchievesFile.c_str()]];
	[dataAccessLock unlock];
}

void GameCenterCommunity::AuthenticatePlayer()
{
	Log::LogT("Authenticating player");
	
	GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
	if (localPlayer == nil)
	{
		assert(false && "Couldn't create GKLocalPlayer object");
		Log::LogE("Couldn't create GKLocalPlayer object");
		return;
	}
	
	[localPlayer authenticateWithCompletionHandler:^(NSError *error) {
		if (localPlayer.isAuthenticated)
		{
			Log::LogT("Player authenticated: %s (%s)", [localPlayer.alias UTF8String], [localPlayer.playerID UTF8String]);
			
			[dataAccessLock lock];
			isAuthenticated = true;
			playerId = [localPlayer.playerID UTF8String];
			[dataAccessLock unlock];
			
			ObsCast(IGameCommunityObserver, this)->NotifyObservers(&IGameCommunityObserver::PlayerAuthenticated, playerId);
			
			LoadOfflineAchievements();
			LoadAchievementsFromServer();
			LoadPointsFromServer();
		}
		else
		{
			Log::LogW("Player couldn't be authenticated");
			
			[dataAccessLock lock];
			isAuthenticated = false;
			playerId = "";
			[dataAccessLock unlock];
	 
			ObsCast(IGameCommunityObserver, this)->NotifyObservers(&IGameCommunityObserver::PlayerDeauthenticated);
		}
	}];
	
	Log::LogT("Finished authentcating player");
}

void GameCenterCommunity::ReportPoints(const std::string &ladderBoardName,
									   unsigned points)
{
	Log::LogT("Reporting %d points to ladderboard %s", points, ladderBoardName.c_str());
	
	if (!IsAuthenticated())
	{
		Log::LogW("Player is not authenticated");
		return;
	}
	
	[dataAccessLock lock];
	if (points <= ladMainPoints)
	{
		Log::LogT("No need to update points (actual is %d)", ladMainPoints);
		[dataAccessLock unlock];
		return;
	}
	[dataAccessLock unlock];
	
	GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:[NSString stringWithUTF8String:ladderBoardName.c_str()]] autorelease];
	if (scoreReporter == nil)
	{
		assert(false && "Couldn't create GKLocalPlayer object");
		Log::LogE("Couldn't create GKLocalPlayer object");
		return;
	}
		
	scoreReporter.value = points;
	[scoreReporter reportScoreWithCompletionHandler:^(NSError *error)
	{
		if (error == nil)
		{
			[dataAccessLock lock];
			ladMainPoints = points;
			[dataAccessLock unlock];
			
			Log::LogT("Scores sended to server");
		}
		else
		{
			Log::LogE("Couldn't report points. Error: %s", [[error localizedDescription] UTF8String]);
		}
	}];
}

void GameCenterCommunity::ReportAchievement(const std::string &achievementName) // do not use achievementName in block!!
{	
	Log::LogT("######################## Sending achievement: %s", achievementName.c_str());
	
	if (!IsAuthenticated())
	{
		Log::LogW("Player is not authenticated");
		return;
	}
	
	[dataAccessLock lock];
	if (srvAchievements == nil)
		srvAchievements = [[NSMutableDictionary alloc] init];
	[dataAccessLock unlock];
	
	if (HasAvievement(achievementName))
	{
		Log::LogT("Already has achievement %s", achievementName.c_str());
		return;
	}
	
	GKAchievement *achievement =
		[[[GKAchievement alloc] initWithIdentifier:[NSString stringWithUTF8String:achievementName.c_str()]] autorelease];
	if (achievement == nil)
	{
		assert(false && "Couldn't create GKAchievement object");
		Log::LogE("Couldn't create GKAchievement object");
		return;
	}
		 
	achievement.percentComplete = 100.0f;
	
	if (Environment::GetInstance()->GetIOSVersion() >= "5.0")
		achievement.showsCompletionBanner = TRUE;
	
	NSString *nsAchName = [NSString stringWithUTF8String:achievementName.c_str()]; // make local copy, to use it inside a block
	[achievement reportAchievementWithCompletionHandler:^(NSError *error)
	{
		if (error == nil)
		{
			Log::LogT("Achievement %s sended sucessfully", [nsAchName UTF8String]);
			
//			if (errAchievements == nil)
//				errAchievements = [[NSMutableDictionary alloc] init]; // test
			[dataAccessLock lock];
			[srvAchievements setObject:achievement forKey:nsAchName];
			[dataAccessLock unlock];
			
			//StoreOfflineAchievements(); // TEST
		}
		else
		{
			Log::LogE("Couldn't send achievement %s to server: %s",
					  [nsAchName UTF8String],
					  [[error localizedDescription] UTF8String]);
			
			[dataAccessLock lock];
			if (errAchievements == nil)
				errAchievements = [[NSMutableDictionary alloc] init];
			[errAchievements setObject:achievement forKey:nsAchName];
			[dataAccessLock unlock];
			
			if (IsAuthenticated())
				StoreOfflineAchievements();
		}
	}];
}

bool GameCenterCommunity::IsAuthenticated() const
{
	[dataAccessLock lock];
	bool ret = isAuthenticated;
	[dataAccessLock unlock];
	
	return ret;
}

void GameCenterCommunity::debug_ClearAchievements()
{	
	[dataAccessLock lock];
	
	if (errAchievements != nil)
	{
		[errAchievements removeAllObjects];
		[errAchievements release];
		errAchievements = nil;
	}
	
	if (srvAchievements != nil)
	{
		[srvAchievements removeAllObjects];
		[srvAchievements release];
		srvAchievements = nil;
	}
	
	[GKAchievement resetAchievementsWithCompletionHandler:^(NSError *error) {
		Log::LogT("debug: achievements resetted");
	}];

	if (!storedAchievesFile.empty())
	{
		NSError *err = nil;	
		if (Path::IsFileExists(storedAchievesFile.c_str()))
			[[NSFileManager defaultManager] removeItemAtPath:[NSString stringWithUTF8String:storedAchievesFile.c_str()] error:&err];
	}

	[dataAccessLock unlock];
}

bool GameCenterCommunity::HasAvievement(const std::string &name)
{
	[dataAccessLock lock];

	NSString *nsName = [NSString stringWithUTF8String:name.c_str()];	
	bool result = [errAchievements objectForKey:nsName] != nil || [srvAchievements objectForKey:nsName] != nil;
	
	[dataAccessLock unlock];
	
	return result;
}

void GameCenterCommunity::ShowLeaderboard()
{
	gameCenterViewProxy->ShowLeaderboard();
}

void GameCenterCommunity::ShowAchievements()
{
	gameCenterViewProxy->ShowAchievements();
}
*/
