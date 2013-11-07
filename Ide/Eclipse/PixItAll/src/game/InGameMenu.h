#pragma once

#include "ITouchObserver.h"
#include "AnimButton.h"

class PlayGameState;
class ToolsPanel;

class InGameMenu :
	public Control,
	public ITouchObserver
{
private:	
	AnimButton *btnBack;
	AnimButton *btnPlayAgain;
	AnimButton *btnMainMenu;
	AnimButton *btnOptions;
	AnimButton *btnPause;
	ToolsPanel *toolsPanel;
	
	unsigned resWidth;
	unsigned resHeight;
	
	bool isActive;
	
	PlayGameState *playGameState;
	
	InGameMenu();
	
	void OnDraw(float time, float seconds);
	void TouchPressed(Control *control, int x, int y);	
	
public:
	~InGameMenu();
	
	static InGameMenu *Create(PlayGameState *playGameState,
							  ToolsPanel *toolsPanel);
	bool IsActive() const;
	void SetActive(bool active);
};
