#include "SelectLevelGameState.h"
#include <stddef.h>
#include "Game.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include "SelectLevelPanel.h"
#include "ImagesCollection.h"
#include "ImageInfo.h"
#include "PlayGameState.h"
#include "MainMenuGameState.h"
#include "MessageBoxManager.h"
#include "LevelStats.h"
#include "Options.h"
#include <sstream>
#include "AchievementsManager.h"

#include <GraphicsLibrary/OpenglPort.h>
using namespace Tizen::Graphics::Opengl;

const std::string SelectLevelGameState::RateUrl = "itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=488585593";

SelectLevelGameState::SelectLevelGameState(Game *game,
										   ImagesCollection *imgColl,
										   IGameCenterViewProxy *viewProxy) :
	BaseGameState(game)
{
	sb = NULL;
	selLvlPanel = NULL;
	this->imgColl = imgColl;
	this->viewProxy = viewProxy;
}

Control *SelectLevelGameState::GetMainPanel()
{
	return selLvlPanel;
}

void SelectLevelGameState::Update(float time, float seconds)
{
	selLvlPanel->Update(time, seconds);
}

void SelectLevelGameState::Draw(float time, float seconds)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	sb->Begin();
	selLvlPanel->Draw(time, seconds);
	sb->End();
}

bool SelectLevelGameState::Initialize()
{
	sb = new SpriteBatch();
	selLvlPanel = SelectLevelPanel::Create(this, imgColl);
	return true;
}

void SelectLevelGameState::Release()
{
	if (sb != NULL)
		delete sb;
}

void SelectLevelGameState::HandleTapGesture(const sm::Point<int> &point)
{
	selLvlPanel->HandleTapGesture(point);
}

void SelectLevelGameState::HandlePress(const sm::Point<int> &point)
{
	selLvlPanel->HandlePress(point);
}

void SelectLevelGameState::HandleRelease(const sm::Point<int> &point)
{
	selLvlPanel->HandleRelease(point);
}

void SelectLevelGameState::HandlePanGesture(IGestureHandler::GestureStatus status,
											const sm::Vec2 &pos,
											const sm::Vec2 &trans,
											const sm::Vec2 &velocity)
{
	selLvlPanel->HandlePanGesture(status, pos, trans, velocity);
}

void SelectLevelGameState::LevelSelected(ImageInfo *imgInfo)
{
	if (!imgInfo->GetLevelStats()->IsUnlocked())
	{
		std::stringstream ss;
		ss << "Level is locked!\nYou still have ";
		ss << ImagesCollection::UnlockedNeverPlayed;
		ss << "\nunresolved levels.";
		MessageBoxManager::GetInstance()->Show(ss.str(), NULL, MessageBox::Buttons_Ok);
	}
	else
	{
		game->playGameState->StartNewGame(imgInfo);
		game->SetGameState(game->playGameState);
	}
}

void SelectLevelGameState::BackToMainMenu()
{
	game->SetGameState(game->mainMenuGameState);
}

void SelectLevelGameState::ScrollToFirstUnlockedUnresolvedLevel()
{
	selLvlPanel->ScrollToFirstUnlockedUnresolvedLevel();
}

void SelectLevelGameState::SetFocus()
{
	Options *opt = Options::GetInstance();
	
	if (opt->GetAskForRate() &&
		opt->GetAskForRateAfterFinishedLevels() <= imgColl->GetFinishedLevelsCount())
	{
		MessageBoxManager::GetInstance()->Show("If you like this\ngame, please rate it!", this, MessageBox::Buttons_OkCancel);
	}
}

void SelectLevelGameState::PropertyChanged(const std::string &propName, void *sender)
{
	if (sender == MessageBoxManager::GetInstance()->GetMessageBox())
	{
		Options::GetInstance()->SetAskForRate(false);
		Options::GetInstance()->Save();
		
		if (propName == "closed" &&
			MessageBoxManager::GetInstance()->GetResult() == MessageBox::Result_Ok)
		{
			viewProxy->ShowAppRatePage(RateUrl);
			AchievementsManager::GetInstance()->UserRatedApp();
		}
	}
}
