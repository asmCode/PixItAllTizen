#pragma once

#include "IGameCenterViewProxy.h"

class DummyGameCenterViewProxy : public IGameCenterViewProxy
{
public:
	void ShowLeaderboard();
	void ShowAchievements();
	void ShowAppRatePage(const std::string &url);
	void ShowHomePage();
};
