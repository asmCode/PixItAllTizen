#pragma once

#include <string>
#include "ObserverSubject.h"
#include "IGameCommunityObserver.h"

class IGameCommunity :
	public ObserversContainer<IGameCommunityObserver*>
{
public:
	virtual ~IGameCommunity() {};
	
	virtual void AuthenticatePlayer() = 0;
	virtual void ReportPoints(const std::string &ladderBoardName,
							  unsigned points) = 0;
	virtual void ReportAchievement(const std::string &achievementName) = 0;
	virtual bool IsAuthenticated() const = 0;
	
	virtual void SetStoredAchievesFile(const std::string &storedAchievesFile) = 0;
	
	virtual void ShowLeaderboard() = 0;
	virtual void ShowAchievements() = 0;
	
	virtual void debug_ClearAchievements() = 0;
};
