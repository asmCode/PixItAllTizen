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
	AnimButton *optionsBtn;
	
	AnimButton *m_topTab;
	AnimButton *m_youTab;
		
	MainMenuGameState *mmGameState;
	
	MainMenuPanel();
	
	void PropertyChanged(const std::string &propName, void *sender);
	void TouchPressed(Control *control, int x, int y);
	void OnDraw(float time, float seconds);
	
	void LeaderOffline();
	void LeaderTopLoaded();
	void LeaderPlayerLoaded();
	void PointsUpdated(int playerId);

public:
	LeaderboardControl* m_leaderboard;

	static MainMenuPanel *Create(MainMenuGameState *mmGameState);
	void SetGameCenterButtons(bool enabled);
};
