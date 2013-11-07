#if 0
#pragma once

#include "IGameCommunity.h"
#include "ObserverSubject.h"
#include <string>

class IGameCenterViewProxy;

class GameCenterCommunity : public IGameCommunity
{
private:
	bool isAuthenticated;
	
	std::string storedAchievesFile;
	
	int reportingAchievesLeft;			//!< number of achievements to report
		
	NSMutableDictionary *errAchievements;
	NSMutableDictionary *srvAchievements;
	
	IGameCenterViewProxy *gameCenterViewProxy;
	
	std::string playerId;
	
	unsigned ladMainPoints; // main points downladed from server
	
	NSLock *dataAccessLock;
	
	void LoadOfflineAchievements();
	
	void LoadAchievementsFromServer();
	void LoadPointsFromServer();
	
	/** Send to server achievements from errAchievements array and remove
		from this array those, who was sucessfully sent */
	void ReportOfflineAchievements();
	
	/** Saves offline errAchievements to local file */
	void StoreOfflineAchievements();
	
	bool HasAvievement(const std::string &name);
	
public:
	GameCenterCommunity(IGameCenterViewProxy *gameCenterViewProxy);
	
	void SetStoredAchievesFile(const std::string &storedAchievesFile);
	
	void AuthenticatePlayer();
	void ReportPoints(const std::string &ladderBoardName,
							  unsigned points);
	void ReportAchievement(const std::string &achievementName);
	
	bool IsAuthenticated() const;
	
	void ShowLeaderboard();
	void ShowAchievements();
	
	void debug_ClearAchievements();
};
#endif
