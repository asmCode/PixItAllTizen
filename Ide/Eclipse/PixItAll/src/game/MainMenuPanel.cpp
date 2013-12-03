#include "MainMenuPanel.h"
#include "ClassContainer.h"
#include "Environment.h"
#include <GraphicsLibrary/TexPart.h>
#include "MainMenuGameState.h"
#include "MessageBox.h"
#include "SoundManager.h"
#include "LeaderboardControl.h"
#include "Leaderboard.h"
#include "PlayerData.h"
#include "ImagesCollection.h"
#include "ScreenKeyboard.h"
#include "PlayerData.h"
#include "XMLElement.h"

template<typename T> extern T* GenericSingleton<T>::instance;

MainMenuPanel::MainMenuPanel()
{
	playGameBtn = NULL;
	optionsBtn = NULL;
}

MainMenuPanel *MainMenuPanel::Create(MainMenuGameState *mmGameState)
{	
	ClassContainer *cc = ClassContainer::GetInstance();
	
	MainMenuPanel *ret = new MainMenuPanel();
	if (ret != NULL)
	{
		ret->mmGameState = mmGameState;
		
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = PixItAll::Environment::GetInstance()->GetScreenWidth();
		ret ->height = PixItAll::Environment::GetInstance()->GetScreenHeight();

		TexPart playGameBtnTexPart = cc ->GetClass<TexPart>("guimap_mm_play_game_btn");
		TexPart playGameDownBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_play_game_down_btn");

		TexPart optionsBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_options_btn");
		TexPart optionsDownBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_options_down_btn");
		
		TexPart tpTop = cc ->GetClass<TexPart>("guimap_leaderboard_top");
		TexPart tpTopDown = cc ->GetClass<TexPart>("guimap_leaderboard_top_down");
		TexPart tpYou = cc ->GetClass<TexPart>("guimap_leaderboard_you");
		TexPart tpYouDown = cc ->GetClass<TexPart>("guimap_leaderboard_you_down");

		ret->bg = cc ->GetClass<TexPart>("guimap_main_menu_bg");
		
		XMLElement *guidefMainMenuPanel = cc->GetClass<XMLElement*>("guidef_MainMenuPanel");
		XMLElement *guidefPlayGameButton = guidefMainMenuPanel->GetSubElement(0);
		XMLElement *guidefOptionsButton = guidefMainMenuPanel->GetSubElement(2);
				
		ret ->playGameBtn = new AnimButton(guidefPlayGameButton->GetAttributeInt("posx"),
										   guidefPlayGameButton->GetAttributeInt("posy"),
										   playGameBtnTexPart,
										   playGameDownBtnTexPart);
		
		ret ->optionsBtn = new AnimButton(guidefOptionsButton->GetAttributeInt("posx"),
										  guidefOptionsButton->GetAttributeInt("posy"),
										  optionsBtnTexPart,
										  optionsDownBtnTexPart);
		
		ret->m_leaderboard = LeaderboardControl::Create();
		//ret->m_leaderboard->SetPosition(0, 800);
		ret->AddChild(ret->m_leaderboard);

		ret ->optionsBtn->pushedShift.x += 6;
		ret ->optionsBtn->pushedShift.y += 4;

		int tabsPosX = 50;
		int tabsPosY = 381;

		ret->m_topTab = new AnimButton(tabsPosX, tabsPosY, tpTop, tpTopDown);
		ret->m_youTab = new AnimButton(tabsPosX + tpTop.ImageRect.Width, tabsPosY, tpYou, tpYouDown);
		ret->m_topTab->SetChecked(true);

		ret->AddChild(ret->m_topTab);
		ret->AddChild(ret->m_youTab);

		ObsCast(ITouchObserver, ret->m_topTab)->AddObserver(ret);
		ObsCast(ITouchObserver, ret->m_youTab)->AddObserver(ret);

		ret ->AddChild(ret ->playGameBtn);
		ret ->AddChild(ret ->optionsBtn);
		
		ObsCast(ITouchObserver, ret->playGameBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->optionsBtn) ->AddObserver(ret);
	}
	
	Leaderboard::GetInstance()->AddObserver(ret);

	return ret;
}

void MainMenuPanel::TouchPressed(Control *control, int x, int y)
{
	if (control == playGameBtn)
		mmGameState->ItemSelected(MainMenuGameState::Menu_StartGame);
	else if (control == optionsBtn)
		mmGameState->ItemSelected(MainMenuGameState::Menu_Options);
	else if (control == m_topTab)
	{
		m_topTab->SetChecked(true);
		m_youTab->SetChecked(false);
		m_leaderboard->SetTab(LeaderboardControl::Tab_Top);
		m_leaderboard->RefreshCurrentView();
	}
	else if (control == m_youTab)
	{
		m_topTab->SetChecked(false);
		m_youTab->SetChecked(true);
		m_leaderboard->SetTab(LeaderboardControl::Tab_You);
		m_leaderboard->RefreshCurrentView();
	}
	
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
}

void MainMenuPanel::OnDraw(float time, float seconds)
{
	this->Control::OnDraw(time, seconds);
}

void MainMenuPanel::PropertyChanged(const std::string &propName, void *sender)
{
}

void MainMenuPanel::SetGameCenterButtons(bool enabled)
{
}

void MainMenuPanel::LeaderTopLoaded()
{
	m_youTab->SetVisible(true);
	m_topTab->SetVisible(true);
	m_leaderboard->SetOnline();
	m_leaderboard->SetPlayerStats(Leaderboard::GetInstance()->GetTopLadder());
}

void MainMenuPanel::LeaderPlayerLoaded()
{
	m_youTab->SetVisible(true);
	m_topTab->SetVisible(true);
	m_leaderboard->SetOnline();
	m_leaderboard->SetPlayerStats(Leaderboard::GetInstance()->GetPlayerLadder());
}

void MainMenuPanel::LeaderOffline()
{
	m_youTab->SetVisible(false);
	m_topTab->SetVisible(false);
	m_leaderboard->SetOffline();
}

void MainMenuPanel::PointsUpdated(int playerId)
{
	m_youTab->SetVisible(true);
	m_topTab->SetVisible(true);
	m_leaderboard->SetOnline();
	PlayerData* pd = PlayerData::GetInstance();

	if (pd->m_id != playerId && playerId != 0)
	{
		pd->m_id = playerId;
		pd->Save();
	}
}
