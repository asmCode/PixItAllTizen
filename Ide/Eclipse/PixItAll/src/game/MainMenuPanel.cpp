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
	m_changeNameBtn = NULL;
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
		TexPart tpChangeButton =  cc->GetClass<TexPart>("guimap_change_name");
		TexPart tpChangeButtonDown =  cc->GetClass<TexPart>("guimap_change_name_down");
		
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
		
		ret ->m_changeNameBtn = new AnimButton(
				0,
				500,
				tpChangeButton,
				tpChangeButtonDown);
		
		ret->m_leaderboard = LeaderboardControl::Create();
		//ret->m_leaderboard->SetPosition(0, 800);
		ret->AddChild(ret->m_leaderboard);

		ret ->optionsBtn->pushedShift.x += 6;
		ret ->optionsBtn->pushedShift.y += 4;

		int tabsPosX = 50;
		int tabsPosY = 381;

		ret->m_topTab = new AnimButton(tabsPosX, tabsPosY, tpTop, tpTopDown);
		ret->m_youTab = new AnimButton(tabsPosX + tpTop.ImageRect.Width, tabsPosY, tpYou, tpYouDown);

		ret->AddChild(ret->m_topTab);
		ret->AddChild(ret->m_youTab);

		ObsCast(ITouchObserver, ret->m_topTab)->AddObserver(ret);
		ObsCast(ITouchObserver, ret->m_youTab)->AddObserver(ret);

		ret ->AddChild(ret ->playGameBtn);
		ret ->AddChild(ret ->optionsBtn);
		ret ->AddChild(ret->m_changeNameBtn);
		
		ObsCast(ITouchObserver, ret->playGameBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->optionsBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->m_changeNameBtn)->AddObserver(ret);
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
		Leaderboard::GetInstance()->RefreshTopLadder();
	}
	else if (control == m_youTab)
	{
		Leaderboard::GetInstance()->RefreshSurrLadder(ImagesCollection::Instance->GetTotalPoints());
	}
	else if (control == m_changeNameBtn)
	{
		ScreenKeyboard::GetInstance()->SetText(PlayerData::GetInstance()->m_name);
		ScreenKeyboard::GetInstance()->Show();
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
	m_leaderboard->SetPlayerStats(Leaderboard::GetInstance()->GetTopLadder());
}

void MainMenuPanel::LeaderPlayerLoaded()
{
	m_leaderboard->SetPlayerStats(Leaderboard::GetInstance()->GetPlayerLadder());
}

void MainMenuPanel::PointsUpdated(int playerId)
{
	PlayerData* pd = PlayerData::GetInstance();

	if (pd->m_id != playerId && playerId != 0)
	{
		pd->m_id = playerId;
		pd->Save();
	}
}
