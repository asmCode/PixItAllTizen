#include "PlayGameState.h"
#include "LevelController.h"
#import "BoardManager.h"
#import "UserImage.h"
#import "BoardView.h"
#include "PalettePanel.h"
#include "ToolsPanel.h"
#include "Palette.h"
#include "Utils/Image.h"
#include "RefImage.h"
#include "IPaletteObserver.h"
#include "FailSplash.h"
#include "VictorySplash.h"
#include "FailPanel.h"
#include "Player.h"
#include "ImageInfo.h"
#include "SummaryScreen.h"
#include "InGameMenu.h"
#import <Utils/MemUtils.h>
#include <GraphicsLibrary/SpriteBatch.h>
#include "Game.h"
#include "MainMenuGameState.h"
#include "ImagesCollection.h"
#include "OptionsPanel.h"
#include "MessageBoxManager.h"
#include "Fade2.h"
#include "TutorialPanel.h"
#include "BonusSplash.h"
#include "SelectLevelGameState.h"
#include "Options.h"

template<typename T> extern T* GenericSingleton<T>::instance;

const std::string PlayGameState::MsgBoxConfirmName = "MsgBoxConfirmName";

PlayGameState::PlayGameState(Game *game, ImagesCollection *imgColl, OptionsPanel *optionsPanel) :
	BaseGameState(game)
{
	this->imgColl = imgColl;
	this->optionsPanel = optionsPanel;
	
	boardMng = NULL;
	refImage = NULL;
	baseImage = NULL;
	boardView = NULL;
	palettePanel = NULL;
	palette = NULL;
	toolsPanel = NULL;
	failSplash = NULL;
	victorySplash = NULL;
	failPanel = NULL;;
	summaryScreen = NULL;
	player = NULL;
	lvlCtrl = NULL;
	inGameMenu = NULL;
	isWaitingForFade = false;
	actionAfterFade = ActionAfterFade_None;
	tutPanel = NULL;
	bonusSplash = NULL;
	
	nextLevelImageInfo = NULL;
	isWaitingForFade = false;
}

PlayGameState::~PlayGameState()
{
	Deinitialize();
}

Control *PlayGameState::GetMainPanel()
{
	return boardView;
}

bool PlayGameState::Initialize()
{
	sb = new SpriteBatch();
	
	return true;
}

void PlayGameState::Deinitialize()
{
	MemUtils::DeleteObject(boardMng);
	MemUtils::DeleteObject(refImage);
	MemUtils::DeleteObject(baseImage);
	MemUtils::DeleteObject(boardView);
	MemUtils::DeleteObject(palettePanel);
	MemUtils::DeleteObject(palette);
	MemUtils::DeleteObject(toolsPanel);
	MemUtils::DeleteObject(summaryScreen);
	MemUtils::DeleteObject(inGameMenu);
	MemUtils::DeleteObject(tutPanel);
	MemUtils::DeleteObject(bonusSplash);
	MemUtils::DeleteObject(sb);
}

void PlayGameState::Update(float time, float seconds)
{
	if (isWaitingForFade &&
		Fade2::GetInstance()->IsReady() &&
		actionAfterFade == ActionAfterFade_PlayAgain)
	{
		Fade2::GetInstance()->StartFadeOut(0.4f);
		isWaitingForFade = false;
		actionAfterFade = ActionAfterFade_None;
		nextLevelImageInfo = imgInfo;
	}
	
	if (nextLevelImageInfo != NULL)
	{
		StartNewGame(nextLevelImageInfo);
		return;
	}
	
	if (boardView->IsPrevievActive() && !boardView->IsFreePreview())
	{
		player->AddCredits(-Player::PreviewCoinsPerSecond * seconds);
		
		if (player->GetCredits() == 0.0f)
			lvlCtrl->EndPreview();
	}
	
	boardView ->Update(time, seconds);	
	palettePanel->Update(time, seconds);
	toolsPanel->Update(time, seconds);

	if (boardView->IsTimeTicking() &&
		!inGameMenu->IsActive() &&
		lvlCtrl->GetLevelState() == LevelController::LevelState_Playing)
		player ->Update(time, seconds);
	
	failSplash->Update(time, seconds);
	failPanel->Update(time, seconds);
	bonusSplash->Update(time, seconds);
	
	if (lvlCtrl->GetLevelState() == LevelController::LevelState_FinishedWithSuccess)
	{
		victorySplash->Update(time, seconds);
		
		if (victorySplash->IsReady() && // if victory finished splashing
			!summaryScreen->IsActive()) // if summary screen is not shown yet
		{
			if (!isWaitingForFade)
			{
				Fade2::GetInstance()->StartFadeIn(0.4f);
				isWaitingForFade = true;
			}
			else if (Fade2::GetInstance()->IsReady())
			{
				summaryScreen->SetActive(true);
				isWaitingForFade = false;
			}
		}
	}
	else if (lvlCtrl->GetLevelState() == LevelController::LevelState_FinishedWithFail)
	{
		if (!failSplash->IsActive() &&
			!summaryScreen->IsActive()) // if summary screen is not shown yet
		{
			if (!isWaitingForFade)
			{
				Fade2::GetInstance()->StartFadeIn(0.4f);
				isWaitingForFade = true;
			}
			else if (Fade2::GetInstance()->IsReady())
			{
				summaryScreen->SetActive(true);
				isWaitingForFade = false;
			}
		}
	}
	
	if (summaryScreen->IsActive())
		summaryScreen->Update(time, seconds);
	if (inGameMenu->IsActive())
		inGameMenu->Update(time, seconds);
	
	if (tutPanel != NULL && tutPanel->IsActive() && !summaryScreen->IsActive() && !inGameMenu->IsActive())
		tutPanel->Update(time, seconds);
}

void PlayGameState::Draw(float time, float seconds)
{	
	sb ->Begin();
	boardView->Draw(time, seconds);
	palettePanel->Draw(time, seconds);
	toolsPanel->Draw(time, seconds);
	failSplash->Draw(time, seconds);
	victorySplash->Draw(time, seconds);
	failPanel->Draw(time, seconds);
	bonusSplash->Draw(time, seconds);
	sb ->End();
	
	if (summaryScreen->IsActive())
	{
		sb ->Begin();
		summaryScreen->Draw(time, seconds);
		sb ->End();
	}
	
	if (inGameMenu->IsActive())
	{
		sb ->Begin();
		inGameMenu->Draw(time, seconds);
		sb ->End();
	}
	
	if (tutPanel != NULL && tutPanel->IsActive() && !summaryScreen->IsActive() && !inGameMenu->IsActive())
	{
		sb ->Begin();
		tutPanel->Draw(time, seconds);
		sb ->End();
	}
}
	
void PlayGameState::HandlePinchGesture(GestureStatus status, float scale, float velocity)
{
	if (inGameMenu->IsActive())
		return;
	
	lvlCtrl ->HandlePinchGesture(status, scale, velocity);
}

void PlayGameState::HandlePanGesture(GestureStatus status,
									 const sm::Vec2 &pos,
									 const sm::Vec2 &trans,
									 const sm::Vec2 &velocity)
{
	if (inGameMenu->IsActive())
		return;
	
	lvlCtrl ->HandlePanGesture(status, pos, trans, velocity);
}

void PlayGameState::HandleTapGesture(const sm::Point<int> &point)
{
	lvlCtrl ->HandleTapGesture(point);
}

void PlayGameState::HandlePress(const sm::Point<int> &point)
{
	lvlCtrl->HandlePress(point);
}

void PlayGameState::HandleRelease(const sm::Point<int> &point)
{
	lvlCtrl->HandleRelease(point);
}

bool PlayGameState::StartNewGame(ImageInfo *imgInfo)
{
	nextLevelImageInfo = NULL;
	
	this->imgInfo = imgInfo;
	this->refImage = const_cast<RefImage*>(imgInfo->GetRefImage());
	
	if (failSplash != NULL)
		delete failSplash;
	failSplash = FailSplash::Create(sb);
	if (failSplash == NULL)
	{
		assert(0 && "FailSplash is null");
		Deinitialize();
		return false;
	}
	
	if (victorySplash != NULL)
		delete victorySplash;
	victorySplash = VictorySplash::Create(sb);
	if (victorySplash == NULL)
	{
		assert(0 && "VictorySplash is null");
		Deinitialize();
		return false;
	}
	
	if (bonusSplash != NULL)
		delete bonusSplash;
	bonusSplash = BonusSplash::Create(sb);
	
	if (palette != NULL)
		delete palette;
	palette = new Palette();
	if (palette == NULL)
	{
		Deinitialize();
		return false;
	}
	palette ->SetFromImage(refImage);
	
	if (boardMng != NULL)
		delete boardMng;
	boardMng = new BoardManager(refImage ->GetWidth(), refImage ->GetHeight(), refImage, imgInfo->GetImageMask());
	if (boardMng == NULL)
	{
		Deinitialize();
		return false;
	}
	
	if (palettePanel != NULL)
		delete palettePanel;
	palettePanel = PalettePanel::Create(palette);
	if (palettePanel == NULL)
	{
		Deinitialize();
		return false;
	}
		
	if (player != NULL)
		delete player;
	player = new Player(imgInfo->GetFailLimit());
	player->AddCredits(imgInfo->GetInitialCredits());
	
	if (failPanel != NULL)
		delete failPanel;
	failPanel = FailPanel::Create(sb, imgInfo->GetFailLimit());
	if (failPanel == NULL)
	{
		assert(false);
		Deinitialize();
		return false;
	}
		
	if (summaryScreen != NULL)
		delete summaryScreen;
	summaryScreen = SummaryScreen::Create(imgInfo, this);
	if (summaryScreen == NULL)
	{
		assert(false);
		return false;
	}
	
	if (toolsPanel != NULL)
		delete toolsPanel;
	toolsPanel = ToolsPanel::Create(palettePanel, player, palette->GetActiveColor());	
	if (toolsPanel == NULL)
	{
		Deinitialize();
		return false;
	}
	
	if (inGameMenu != NULL)
		delete inGameMenu;
	inGameMenu = InGameMenu::Create(this, toolsPanel);
	if (inGameMenu == NULL)
	{
		assert(false);
		return false;
	}
		
	if (boardView != NULL)
		delete boardView;
	boardView = new BoardView(boardMng, palette, palettePanel, imgInfo->IsTutorial(), toolsPanel);
	if (boardView == NULL || !boardView ->Initialize())
	{
		Deinitialize();
		return false;
	}
	
	if (tutPanel != NULL)
	{
		delete tutPanel;
		tutPanel = NULL;
	}
	
	if (imgInfo->IsTutorial())
	{
		tutPanel = TutorialPanel::Create((const UserImage*)boardMng->GetUserImage(),
										 toolsPanel,
										 palettePanel,
										 boardView);
		if (tutPanel == NULL)
		{
			Deinitialize();
			return false;
		}
		
		ObsCast(IPropertyObserver, palettePanel)->AddObserver(tutPanel);
		ObsCast(IPropertyObserver, boardView)->AddObserver(tutPanel);
	}
	
	if (lvlCtrl != NULL)
		delete lvlCtrl;
	lvlCtrl = new LevelController(player,
								  imgInfo,
								  imgColl,
								  boardMng,
								  boardView,
								  palettePanel,
								  toolsPanel,
								  failSplash,
								  victorySplash,
								  failPanel,
								  summaryScreen,
								  inGameMenu,
								  tutPanel,
								  bonusSplash);
	
	toolsPanel->SetLevelController(lvlCtrl);
			
	ObsCast(IPaletteObserver, palette) ->AddObserver(palettePanel);
	ObsCast(IPaletteObserver, palette) ->AddObserver(toolsPanel);
	ObsCast(IBoardObserver, boardMng) ->AddObserver(boardView);
	ObsCast(IBoardObserver, boardMng) ->AddObserver(lvlCtrl);
	ObsCast(IPropertyObserver, player) ->AddObserver(toolsPanel);
	
	baseImage = NULL;
	
	if (!boardView ->Initialize())
		return false;
	
	if (!boardMng ->Initialize())
		return false;

	return true;
}

void PlayGameState::BackToMenu()
{
	if (lvlCtrl->GetLevelState() == LevelController::LevelState_Playing)
	{
		MessageBoxManager::GetInstance()->Show("Do you really want to\ngo back to\nmain menu?",
											   this,
											   MsgBoxConfirmName,
											   MsgBoxComfirm_MainMenu);
	}
	else
	{
		game->SetGameState(game->mainMenuGameState);
	}
}

void PlayGameState::PlayAgain()
{
	if (lvlCtrl->GetLevelState() == LevelController::LevelState_Playing)
	{
		MessageBoxManager::GetInstance()->Show("Do you really want\nto play again?",
											   this,
											   MsgBoxConfirmName,
											   MsgBoxComfirm_PlayAgain);
	}
	else
	{
		isWaitingForFade = true;
		actionAfterFade = ActionAfterFade_PlayAgain;
		Fade2::GetInstance()->StartFadeIn(0.4f);
	}
}

void PlayGameState::NextLevel()
{
	nextLevelImageInfo = imgColl->GetNextUnresolvedImage(imgInfo);
	if (nextLevelImageInfo == NULL)
	{
		// if this is first time that we finished all levels
		if (Options::GetInstance()->GetShowFinishedAllLevels())
		{
			std::string msg;
			msg = "Well, yo did it!\nNo more levels left,\nbut if You want more,\n";
			msg += "just let us know. We\nneed your motivation!";
			MessageBoxManager::GetInstance()->Show(msg,
												   this,
												   MsgBoxConfirmName,
												   MsgBoxComfirm_MainMenu,
												   MessageBox::Buttons_Ok);
			Options::GetInstance()->SetShowFinishedAllLevels(false);
			Options::GetInstance()->Save();
		}
		else
		{
			// if not, then just jump to next resolved level
			nextLevelImageInfo = imgColl->GetNextImage(imgInfo);
			if (nextLevelImageInfo == NULL)
			{
				std::string msg;
				msg = "Well, yo did it!\nNo more levels left,\nbut if You want more,\n";
				msg += "just let us know. We\nneed your motivation!";
				MessageBoxManager::GetInstance()->Show(msg,
													   this,
													   MsgBoxConfirmName,
													   MsgBoxComfirm_MainMenu,
													   MessageBox::Buttons_Ok);
			}
		}
	}
}

void PlayGameState::ShowOptions()
{
	optionsPanel->SetActive(true);
}

void PlayGameState::PropertyChanged(const std::string &propName, void *sender)
{
	assert(sender != NULL);
	
	const MessageBox* msgBox = MessageBoxManager::GetInstance()->GetMessageBox();
	
	if (sender == msgBox)
	{
		if (msgBox->GetResult() != MessageBox::Result_Ok)
			return;
		
		MsgBoxComfirm msgBoxConfirm;
		msgBox->GetUserData(MsgBoxConfirmName, msgBoxConfirm);
		
		switch (msgBoxConfirm)
		{
			case MsgBoxComfirm_MainMenu:
				game->SetGameState(game->mainMenuGameState);
				break;
				
			case MsgBoxComfirm_PlayAgain:
				isWaitingForFade = true;
				actionAfterFade = ActionAfterFade_PlayAgain;
				Fade2::GetInstance()->StartFadeIn(0.4f);
				break;
		}
	}
}

void PlayGameState::HandleEnterBackground()
{
	if (lvlCtrl != NULL)
		lvlCtrl->HandleEnterBackground();
}

void PlayGameState::TogglePause()
{
	if (inGameMenu->IsActive())
		inGameMenu->SetActive(false);
	else
		inGameMenu->SetActive(true);
}
