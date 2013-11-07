#if 0
#pragma once

#include <UIKit/UIKit.h>
#include "IGameCenterViewProxy.h"
#import "Repaint_ItViewController.h"

class GameCenterViewProxy : public IGameCenterViewProxy
{
private:
	Repaint_ItViewController *mainViewCtrl;
	
public:
	GameCenterViewProxy(Repaint_ItViewController *mainViewCtrl);
	
	void ShowLeaderboard();
	void ShowAchievements();
	void ShowAppRatePage(const std::string &url);
	void ShowHomePage();
};
#endif
