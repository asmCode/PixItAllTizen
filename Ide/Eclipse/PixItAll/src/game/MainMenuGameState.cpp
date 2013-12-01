#include "MainMenuGameState.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include <stddef.h>
#include "Game.h"
#include "MainMenuPanel.h"
#include "SelectLevelGameState.h"
#include "OptionsPanel.h"
#include "Fade2.h"
#include "ClassContainer.h"
#include "Leaderboard.h"
#include "IGameCommunity.h"
#include "Leaderboard.h"
#include "MessageBoxManager.h"
#include <GraphicsLibrary/OpenglPort.h>
using namespace Tizen::Graphics::Opengl;

MainMenuGameState::MainMenuGameState(Game *game, OptionsPanel *optionsPanel, IGameCenterViewProxy *viewProxy) :
	BaseGameState(game)
{
	sb = NULL;
	mmPanel = NULL;
	this->optionsPanel = optionsPanel;
    this->waitingForFade = false;
    this->actionAfterFade = ActionAfterFade_None;
	this->viewProxy = viewProxy;
}

MainMenuGameState::~MainMenuGameState()
{
	Release();
}

Control *MainMenuGameState::GetMainPanel()
{
	return mmPanel;
}

void MainMenuGameState::Update(float time, float seconds)
{
    if (waitingForFade && Fade2::GetInstance()->IsReady())
    {
        if (actionAfterFade == ActionAfterFade_Options)
            optionsPanel->SetActive(true);
        
        waitingForFade = false;
        actionAfterFade = ActionAfterFade_None;
    }
    
	mmPanel->Update(time, seconds);
}

void MainMenuGameState::Draw(float time, float seconds)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	sb->Begin();
	mmPanel->Draw(time, seconds);
	sb->End();
}

bool MainMenuGameState::Initialize()
{
	sb = new SpriteBatch();
	mmPanel = MainMenuPanel::Create(this);
	
	return true;
}

void MainMenuGameState::Release()
{
	if (sb != NULL)
		delete sb;
	
	if (mmPanel != NULL)
		delete mmPanel;
}

void MainMenuGameState::HandleTapGesture(const sm::Point<int> &point)
{
	mmPanel->HandleTapGesture(point);
}

void MainMenuGameState::HandlePress(const sm::Point<int> &point)
{
	mmPanel->HandlePress(point);
}

void MainMenuGameState::HandleRelease(const sm::Point<int> &point)
{
	mmPanel->HandleRelease(point);
}

void MainMenuGameState::ItemSelected(MainMenuGameState::Menu menu)
{
	switch (menu)
	{
		case Menu_StartGame:
			game->SetGameState(game->selectLevelGameState);
			break;
			
		case Menu_FreePlay:
		{
			/*
			IGameCommunity *gc = ClassContainer::GetInstance()->GetClass<IGameCommunity*>("gc");
			gc->debug_ClearAchievements();
			*/
			
//			MessageBoxManager::GetInstance()->Show("Coming up soon!\nThis mode will allow\nYou to make your own\nImages!", NULL, MessageBox::Buttons_Ok);
			viewProxy->ShowHomePage();
			
		}
			break;
			
		case Menu_Options:
            waitingForFade = true;
            actionAfterFade = ActionAfterFade_Options;
            Fade2::GetInstance()->StartFadeIn(0.4f);
			break;
            
        case Menu_Credits:
            break;
            
        case Menu_VisitPage:
            break;
			
		case Menu_Leaderboard:
		{
			IGameCommunity *gc = ClassContainer::GetInstance()->GetClass<IGameCommunity*>("gc");
			gc->ShowLeaderboard();
            break;
		}
			
		case Menu_Achievements:
		{
			IGameCommunity *gc = ClassContainer::GetInstance()->GetClass<IGameCommunity*>("gc");
			gc->ShowAchievements();
			break;
		}
	}
}

void MainMenuGameState::SetGameCenterButtons(bool enabled)
{
	mmPanel->SetGameCenterButtons(enabled);
}

void MainMenuGameState::SetFocus()
{
	Leaderboard::GetInstance()->RefreshTopLadder();
}
