#pragma once

#include "IGameCommunity.h"
#include <string>

class IGameCenterViewProxy;

class DummyCommunity : public IGameCommunity
{
public:
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
