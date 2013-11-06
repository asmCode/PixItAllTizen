#pragma once

#include "ITouchObserver.h"
#include "IPropertyObserver.h"
#include "Control.h"
#include "AnimButton.h"

class MainMenuGameState;

class MainMenuPanel :
	public Control,
	public ITouchObserver,
	public IPropertyObserver
{
private:
	AnimButton *playGameBtn;
	AnimButton *freePlayBtn;
	AnimButton *optionsBtn;
	AnimButton *leaderBtn;	// leaderboard
	AnimButton *achBtn;		// achievements
	
	Control *imgLogo;
		
	MainMenuGameState *mmGameState;
	
	MainMenuPanel();
	
	void PropertyChanged(const std::string &propName, void *sender);
	void TouchPressed(Control *control, int x, int y);
	void OnDraw(float time, float seconds);
	
public:
	static MainMenuPanel *Create(MainMenuGameState *mmGameState);
	void SetGameCenterButtons(bool enabled);
};
