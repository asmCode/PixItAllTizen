#pragma once

#include "ITouchObserver.h"
#include "IPropertyObserver.h"
#include "Control.h"
#include "AnimButton.h"
#include "ILeaderboardObserver.h"

class MainMenuGameState;
class LeaderboardControl;

class MainMenuPanel :
	public Control,
	public ITouchObserver,
	public IPropertyObserver,
	public ILeaderboardObserver
{
private:
	AnimButton *playGameBtn;
	AnimButton *freePlayBtn;
	AnimButton *optionsBtn;
	AnimButton *leaderBtn;	// leaderboard
	AnimButton *achBtn;		// achievements
	AnimButton *m_changeNameBtn;
	
	LeaderboardControl* m_leaderboard;

	Control *imgLogo;
		
	MainMenuGameState *mmGameState;
	
	MainMenuPanel();
	
	void PropertyChanged(const std::string &propName, void *sender);
	void TouchPressed(Control *control, int x, int y);
	void OnDraw(float time, float seconds);
	
	void LeaderTopLoaded();
	void LeaderPlayerLoaded() {};
	void PointsUpdated(int playerId);

public:
	static MainMenuPanel *Create(MainMenuGameState *mmGameState);
	void SetGameCenterButtons(bool enabled);
};
