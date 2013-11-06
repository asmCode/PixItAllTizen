#include "LevelController.h"
#include "Player.h"
#include "BoardManager.h"
#include "BoardView.h"
#include "PalettePanel.h"
#include "ToolsPanel.h"
#include "FailSplash.h"
#include "FailPanel.h"
#include "UserImage.h"
#include "SummaryScreen.h"
#include "InGameMenu.h"
#include "ImageInfo.h"
#include "ImagesCollection.h"
#include "LevelStats.h"
#include "VictorySplash.h"
#include "IGameCommunity.h"
#include "SoundManager.h"
#include "TutorialPanel.h"
#include "Palette.h"
#include "BonusSplash.h"
#include "AchievementsManager.h"

#include <GraphicsLibrary/OpenglPort.h>

LevelController::LevelController(Player *player,
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
								 BonusSplash *bonusSplash)
{
	this->player = player;
	this->imgInfo = imgInfo;
	this->imgCollection = imgCollection;
	this->boardMng = boardMng;
	this->boardView = boardView;
	this->palettePanel = palettePanel;
	this->toolsPanel = toolsPanel;
	this->failSplash = failSplash;
	this->victorySplash = victorySplash;
	this->failPanel = failPanel;
	this->summaryScreen = summaryScreen;
	this->inGameMenu = inGameMenu;
	this->bonusSplash = bonusSplash;
	
	this->levelState = LevelState_Playing;
	correctPixelCount = 0;
	
	this->tutPanel = tutPanel;
	
	soundMng = ClassContainer::GetInstance()->GetClass<SoundManager*>("soundManager");
	assert(soundMng != NULL);
}

void LevelController::BoardInitialized()
{
	if (imgInfo->IsTutorial())
	{
		palettePanel->GetPalette()->SetColor(2);
		toolsPanel->EnablePaletteButton(false);
		toolsPanel->EnablePreviewButton(false);
	}
	
	toolsPanel->SetCredits(imgInfo->GetInitialCredits());
	AchievementsManager::GetInstance()->GameStarted(0,
													imgInfo->GetWidth(),
													imgInfo->GetHeight(),
													imgInfo->GetColorsCount());
}

void LevelController::PickedWrongColor(int x, int y, const Color &color)
{
	AchievementsManager::GetInstance()->PickedWrongColor();
	
	// no fail if in tutorial mode
	if (tutPanel != NULL && tutPanel->IsActive())
		return;
	
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Fail);
	
	correctPixelCount = 0;
	toolsPanel->ShowDoubleBonus(false);
	boardView->SetBonusPixels(false);
	
	if (player->GetFailLimit() != player->GetFailCount())
	{
		player->AddFail();
		failPanel->MakeFail();
		failSplash->DoSplash();
	}
	
	if (player->GetFailLimit() == player->GetFailCount())
	{
		levelState = LevelState_FinishedWithFail;
		
		summaryScreen->SetFailure();
		
		AchievementsManager::GetInstance()->GameFinished(false);
	}
}

void LevelController::PickedCorrectColor(int x, int y, const Color &color)
{	
	AchievementsManager::GetInstance()->PickedCorrectColor(player->GetTime(), x, y);
	
	player->ResetFails();
	failPanel->ResetFails();
	player->AddCredits(Player::PointsPerPixel * (correctPixelCount >= 20 ? 2 : 1));
	
	correctPixelCount++;
	
	UserImage *userImg = (UserImage*)boardMng->GetUserImage();
	if (userImg->GetTotalPixels() == userImg->GetResolvedPixels())
	//if (true) // TEST
	{
		SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Victory);
		OnLevelComplete();
	}
	else
	{
		if (correctPixelCount == 5)
		{
			bonusSplash->DoSplash(BonusSplash::BonusType_5InARow);
			soundMng->PlayBonus(0);
			player->AddCredits(5);
		}
		else if (correctPixelCount == 10)
		{
			bonusSplash->DoSplash(BonusSplash::BonusType_10InARow);
			soundMng->PlayBonus(1);
			player->AddCredits(10);
		}
		else if (correctPixelCount == 20)
		{
			bonusSplash->DoSplash(BonusSplash::BonusType_20InARow);
			toolsPanel->ShowDoubleBonus(true);
			boardView->SetBonusPixels(true);
			soundMng->PlayBonus(2);
		}
		else
		{
			soundMng->PlayPixel();
		}
	}
}

void LevelController::HandlePinchGesture(IGestureHandler::GestureStatus status,
										 float scale,
										 float velocity)
{
	if (CheckBoardManipulation() &&
		levelState == LevelState_Playing &&
		palettePanel->IsRolled() &&
		(tutPanel == NULL || (tutPanel != NULL && tutPanel->GetLesson() >= TutorialPanel::Lesson_ZoomMove)))
		boardView ->HandlePinchGesture(status, scale, velocity);	
}

void LevelController::HandlePanGesture(IGestureHandler::GestureStatus status,
									   const sm::Vec2 &pos,
									   const sm::Vec2 &trans,
									   const sm::Vec2 &velocity)
{
	if (!palettePanel->IsRolled() && levelState == LevelState_Playing)
	{
		palettePanel->HandlePanGesture(status, pos, trans, velocity);
		return;
	}
	
	if (CheckBoardManipulation() && levelState == LevelState_Playing &&
		(tutPanel == NULL || (tutPanel != NULL && tutPanel->GetLesson() >= TutorialPanel::Lesson_ZoomMove)))
		boardView ->HandlePanGesture(status, pos, trans, velocity);

	if (levelState == LevelState_Playing)
	{
		palettePanel->HandlePanGesture(status, pos, trans, velocity);
		toolsPanel->HandlePanGesture(status, pos, trans, velocity);
	}
}

void LevelController::HandleTapGesture(const sm::Point<int> &point)
{
	if (inGameMenu->IsActive())
		inGameMenu->HandleTapGesture(point);
	else if (summaryScreen->IsActive())
		summaryScreen->HandleTapGesture(point);
	else if (!palettePanel->IsRolled() && levelState == LevelState_Playing)
		palettePanel->HandleTapGesture(point);
	else if (CheckBoardManipulation())
	{
		if (levelState == LevelState_Playing)
		{
			if (tutPanel == NULL || !tutPanel->IsActive())
			{
				boardView->HandleTapGesture(point);
			}
			else
			{
				tutPanel->HandleTapGesture(point);
				
				if (tutPanel->GetLesson() >= TutorialPanel::Lesson_FourColors)
					boardView->HandleTapGesture(point);
			}
			
			toolsPanel->HandleTapGesture(point);
		}
		else if (levelState == LevelState_FinishedWithSuccess)
		{
			victorySplash->Cancel();
		}
	}
}

void LevelController::HandlePress(const sm::Point<int> &point)
{
	if (inGameMenu->IsActive())
		inGameMenu->HandlePress(point);
	else if (summaryScreen->IsActive())
		summaryScreen->HandlePress(point);
	else if (!palettePanel->IsRolled() && levelState == LevelState_Playing)
		palettePanel->HandlePress(point);
	else if (levelState == LevelState_Playing)
	{
		toolsPanel->HandlePress(point);
		boardView->HandlePress(point);
	}
}

void LevelController::HandleRelease(const sm::Point<int> &point)
{
	if (inGameMenu->IsActive())
		inGameMenu->HandleRelease(point);
	else if (summaryScreen->IsActive())
		summaryScreen->HandleRelease(point);
	else if (!palettePanel->IsRolled())
		palettePanel->HandleRelease(point);
	else if (levelState == LevelState_Playing)
	{
		toolsPanel->HandleRelease(point);
		boardView->HandleRelease(point);
	}
}

bool LevelController::CheckBoardManipulation()
{
	if (failSplash->IsActive())
	{
		if (!failSplash->IsStopping())
			failSplash->StopSplash();
		
		return false;
	}
	
	return true;
}

void LevelController::StartPreview()
{
	if (player->GetCredits() > 0.0f)
	{
		boardView->ShowPreview();
		AchievementsManager::GetInstance()->PreviewShown();
	}
	else
		toolsPanel->NotifyNoMoney();
}

void LevelController::EndPreview()
{
	boardView->HidePreview();
}

void LevelController::ShowInGameMenu()
{
	inGameMenu->SetActive(true);
}

void LevelController::OnLevelComplete()
{
	AchievementsManager::GetInstance()->GameFinished(true);
	
	if (imgInfo->IsTutorial())
		AchievementsManager::GetInstance()->FinishedTutorial();
	
	levelState = LevelState_FinishedWithSuccess;
	
	int points = player->CalcPoints(imgInfo->GetWidth(),
									imgInfo->GetHeight(),
									imgInfo->GetColorsCount());
	
	bool isRecord = false;
	if (points > imgInfo->GetLevelStats()->GetPoints())
	{
		imgInfo->GetLevelStats()->SetPoints(points);
		imgInfo->GetLevelStats()->Save();
		
		isRecord = true;
	}
	
	imgCollection->UnlockLevels();
	
	IGameCommunity *gc = ClassContainer::GetInstance()->GetClass<IGameCommunity*>("gc");
	if (gc == NULL)
	{
		assert(false);
		return;
	}
	
	gc->ReportPoints("pixitall.leaderboard.main", imgCollection->GetTotalPoints());
	
	boardView->ZoomOutMax();
	victorySplash->DoSplash();
	
	summaryScreen->SetSuccess(player->GetTime(),
							  player->GetCredits(),
							  player->GetTotalFailCount(),
							  points,
							  isRecord);
}

LevelController::LevelState LevelController::GetLevelState() const
{
	return levelState;
}

void LevelController::CloseVictoryScreen()
{
	if (levelState == LevelState_FinishedWithSuccess)
		victorySplash->Cancel();
}

void LevelController::HandleEnterBackground()
{
	if (boardView != NULL && boardView->IsPrevievActive())
		boardView->HidePreview();
}
