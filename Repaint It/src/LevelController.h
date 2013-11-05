#pragma once

#include "IBoardObserver.h"
#include "IGestureHandler.h"

class Player;
class BoardManager;
class BoardView;
class FailSplash;
class FailPanel;
class SummaryScreen;
class InGameMenu;
class PalettePanel;
class ToolsPanel;
class ImageInfo;
class ImagesCollection;
class VictorySplash;
class SoundManager;
class TutorialPanel;
class BonusSplash;

class LevelController :
	public IBoardObserver,
	public IGestureHandler
{
public:
	enum LevelState
	{
		LevelState_Playing,
		LevelState_FinishedWithSuccess,
		LevelState_FinishedWithFail,
	};
	
private:	
	Player			*player;
	BoardManager	*boardMng;
	BoardView		*boardView;
	FailSplash		*failSplash;
	VictorySplash	*victorySplash;
	FailPanel		*failPanel;
	SummaryScreen	*summaryScreen;
	InGameMenu		*inGameMenu;
	PalettePanel	*palettePanel;
	ToolsPanel		*toolsPanel;
	ImageInfo		*imgInfo;
	ImagesCollection	*imgCollection;
	SoundManager	*soundMng;
	TutorialPanel	*tutPanel;
	BonusSplash		*bonusSplash;
	
	LevelState		levelState;
	
	unsigned		correctPixelCount;
	
	bool CheckBoardManipulation();
	
	void OnLevelComplete();

public:
	LevelController(Player *player,
					ImageInfo *imgInfo,
					ImagesCollection *imgCollection,
					BoardManager *boardMng,
					BoardView *boardView,
					PalettePanel *palettePanel,
					ToolsPanel *toolsPanel,
					FailSplash *failSplash,
					VictorySplash *victorySplash,
					FailPanel *failPanel,
					SummaryScreen *summaryScreen,
					InGameMenu *inGameMenu,
					TutorialPanel *tutPanel,
					BonusSplash *bonusSplash);
					
	
	// IBoardObserver members
	void BoardInitialized();
	void PickedWrongColor(int x, int y, const Color &color);
	void PickedCorrectColor(int x, int y, const Color &color);
	
	// IGestureHandler
	void HandlePinchGesture(IGestureHandler::GestureStatus status,
							float scale,
							float velocity);
	
	void HandlePanGesture(IGestureHandler::GestureStatus status,
						  const sm::Vec2 &pos,
						  const sm::Vec2 &trans,
						  const sm::Vec2 &velocity);
	
	void HandleTapGesture(const sm::Point<int> &point);
	void HandlePress(const sm::Point<int> &point);
	void HandleRelease(const sm::Point<int> &point);
	
	void StartPreview();
	void EndPreview();
	
	void ShowInGameMenu();
	LevelState GetLevelState() const;
	
	void CloseVictoryScreen();
	
	void HandleEnterBackground();
};
