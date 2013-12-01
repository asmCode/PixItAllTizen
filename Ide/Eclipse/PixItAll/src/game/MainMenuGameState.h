#pragma once

#include "BaseGameState.h"
#include "IGameCenterViewProxy.h"

class SpriteBatch;
class MainMenuPanel;
class OptionsPanel;

class MainMenuGameState : public BaseGameState
{
public:
	enum Menu
	{
		Menu_StartGame,
		Menu_FreePlay,
		Menu_Options,
		Menu_Credits,
		Menu_VisitPage,
		Menu_Leaderboard,
		Menu_Achievements
	};
    
    enum ActionAfterFade
    {
        ActionAfterFade_None,
        ActionAfterFade_Options
    };

private:
	SpriteBatch *sb;
	MainMenuPanel *mmPanel;
	OptionsPanel *optionsPanel;
    
    bool            waitingForFade;
    ActionAfterFade actionAfterFade;
	IGameCenterViewProxy *viewProxy;
	
	void SetFocus();

public:
	MainMenuGameState(Game *game, OptionsPanel *optionsPanel, IGameCenterViewProxy *viewProxy);
	~MainMenuGameState();
	
	Control *GetMainPanel();
	
	void Update(float time, float seconds);
	void Draw(float time, float seconds);
	
	void HandleTapGesture(const sm::Point<int> &point);
	void HandlePress(const sm::Point<int> &point);
	void HandleRelease(const sm::Point<int> &point);	
	
	bool Initialize();
	void Release();
	
	void ItemSelected(Menu menu);
	
	void SetGameCenterButtons(bool enabled);
};
