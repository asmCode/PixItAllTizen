#pragma once

#include "BaseGameState.h"
#include "IPropertyObserver.h"
#include <string>

class SpriteBatch;
class LevelController;
class BoardManager;
class BoardView;
class IImage;
class PalettePanel;
class ToolsPanel;
class Palette;
class Player;
class FailSplash;
class VictorySplash;
class FailPanel;
class ImageInfo;
class SummaryScreen;
class ImageInfo;
class ImagesCollection;
class InGameMenu;
class OptionsPanel;
class TutorialPanel;
class BonusSplash;

class PlayGameState :
	public BaseGameState,
	public IPropertyObserver
{
private:
	enum MsgBoxComfirm
	{
		MsgBoxComfirm_PlayAgain,
		MsgBoxComfirm_MainMenu
	};
	
	enum ActionAfterFade
	{
		ActionAfterFade_None,
		ActionAfterFade_PlayAgain
	};
	
	bool			isWaitingForFade;
	ActionAfterFade	actionAfterFade;
	
	static const std::string MsgBoxConfirmName;
	
	SpriteBatch *sb;
	
	BoardManager	*boardMng;
	IImage			*refImage;
	IImage			*baseImage;
	BoardView		*boardView;
	Player			*player;
	FailSplash		*failSplash;
	VictorySplash	*victorySplash;
	FailPanel		*failPanel;
	
	ToolsPanel		*toolsPanel;
	PalettePanel	*palettePanel;
	Palette			*palette;
	SummaryScreen	*summaryScreen;
	InGameMenu		*inGameMenu;
	OptionsPanel	*optionsPanel;
	BonusSplash		*bonusSplash;
	
	LevelController	*lvlCtrl;
	ImageInfo		*imgInfo;
	
	ImagesCollection	*imgColl;
	TutorialPanel	*tutPanel;
	
	// if not null, then in next Update change level to this ImageInfo
	ImageInfo *nextLevelImageInfo;
	
	void PropertyChanged(const std::string &propName, void *sender);
	
public:
	PlayGameState(Game *game, ImagesCollection *imgColl, OptionsPanel *optionsPanel);
	~PlayGameState();
	
	Control *GetMainPanel();
	
	void Update(float time, float seconds);
	void Draw(float time, float seconds);
	
	void HandlePinchGesture(GestureStatus status, float scale, float velocity);
	void HandleTapGesture(const sm::Point<int> &point);
	void HandlePanGesture(IGestureHandler::GestureStatus status,
						  const sm::Vec2 &pos,
						  const sm::Vec2 &trans,
						  const sm::Vec2 &velocity);
	void HandlePress(const sm::Point<int> &point);	
	void HandleRelease(const sm::Point<int> &point);	
	
	bool Initialize();
	void Deinitialize();

	bool StartNewGame(ImageInfo *imgInfo);
	
	void BackToMenu();
	void PlayAgain();
	void ShowOptions();
	void NextLevel();
	
	void HandleEnterBackground();
};
