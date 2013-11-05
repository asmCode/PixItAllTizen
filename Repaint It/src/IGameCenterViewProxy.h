#pragma once

#include <string>

class IGameCenterViewProxy
{
public:
	virtual void ShowLeaderboard() = 0;
	virtual void ShowAchievements() = 0;
	virtual void ShowAppRatePage(const std::string &url) = 0;
	virtual void ShowHomePage() = 0;
};
