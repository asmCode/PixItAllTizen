#include "GameCenterViewProxy.h"
#include <GameKit/GameKit.h>
#include <UIKit/UIApplication.h>
#include "Log.h"

GameCenterViewProxy::GameCenterViewProxy(Repaint_ItViewController *mainViewCtrl)
{
	this->mainViewCtrl = mainViewCtrl;
}

void GameCenterViewProxy::ShowLeaderboard()
{
	GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
	if (leaderboardController != nil)
	{
		leaderboardController.leaderboardDelegate = mainViewCtrl;
		[mainViewCtrl presentModalViewController: leaderboardController animated: YES];
	}
	[leaderboardController release];
}

void GameCenterViewProxy::ShowAchievements()
{
	GKAchievementViewController *achievements = [[GKAchievementViewController alloc] init];
	if (achievements != nil)
	{
		achievements.achievementDelegate = mainViewCtrl;
		[mainViewCtrl presentModalViewController: achievements animated: YES];
	}
	[achievements release];
}

void GameCenterViewProxy::ShowAppRatePage(const std::string &url)
{
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
}

void GameCenterViewProxy::ShowHomePage()
{
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:"http://pixitall.semiseriousgames.com"]]];
}