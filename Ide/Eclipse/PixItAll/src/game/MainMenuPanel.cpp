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
#include "ScreenKeyboard.h"
#include "PlayerData.h"
#include "XMLElement.h"

template<typename T> extern T* GenericSingleton<T>::instance;

MainMenuPanel::MainMenuPanel()
{
	playGameBtn = NULL;
	freePlayBtn = NULL;
	optionsBtn = NULL;
	leaderBtn = NULL;
	achBtn = NULL;
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
		
		// TODO: i cant use free play mode on app store
//		TexPart freePlayBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_free_play_btn");
//		TexPart freePlayDownBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_free_play_down_btn");
		TexPart freePlayBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_home_page_btn");
		TexPart freePlayDownBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_home_page_down_btn");
		
		TexPart optionsBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_options_btn");
		TexPart optionsDownBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_options_down_btn");
		TexPart leaderBtnTexPart =  cc->GetClass<TexPart>("guimap_leader");
		TexPart leaderDownBtnTexPart =  cc->GetClass<TexPart>("guimap_leader_down");
		TexPart achBtnTexPart =  cc->GetClass<TexPart>("guimap_ach");
		TexPart achDownBtnTexPart =  cc->GetClass<TexPart>("guimap_ach_down");
		TexPart tpLogoPixel =  cc->GetClass<TexPart>("guimap_logo_pixel");
		TexPart tpChangeButton =  cc->GetClass<TexPart>("guimap_change_name");
		TexPart tpChangeButtonDown =  cc->GetClass<TexPart>("guimap_change_name_down");
		
		ret->bg = cc ->GetClass<TexPart>("guimap_main_menu_bg");
		
		XMLElement *guidefMainMenuPanel = cc->GetClass<XMLElement*>("guidef_MainMenuPanel");
		XMLElement *guidefPlayGameButton = guidefMainMenuPanel->GetSubElement(0);
		XMLElement *guidefFreePlayButton = guidefMainMenuPanel->GetSubElement(1);
		XMLElement *guidefOptionsButton = guidefMainMenuPanel->GetSubElement(2);
		XMLElement *guidefLeaderboardButton = guidefMainMenuPanel->GetSubElement(3);
		XMLElement *guidefAchievementsButton = guidefMainMenuPanel->GetSubElement(4);
		XMLElement *guidefLogo = guidefMainMenuPanel->GetSubElement(5);
				
		ret ->playGameBtn = new AnimButton(guidefPlayGameButton->GetAttributeInt("posx"),
										   guidefPlayGameButton->GetAttributeInt("posy"),
										   playGameBtnTexPart,
										   playGameDownBtnTexPart);
		
		ret ->freePlayBtn = new AnimButton(guidefFreePlayButton->GetAttributeInt("posx"),
										   guidefFreePlayButton->GetAttributeInt("posy"),
										   freePlayBtnTexPart,
										   freePlayDownBtnTexPart);
		
		ret ->optionsBtn = new AnimButton(guidefOptionsButton->GetAttributeInt("posx"),
										  guidefOptionsButton->GetAttributeInt("posy"),
										  optionsBtnTexPart,
										  optionsDownBtnTexPart);
		
		ret ->leaderBtn = new AnimButton(guidefLeaderboardButton->GetAttributeInt("posx"),
										 guidefLeaderboardButton->GetAttributeInt("posy"),
										 leaderBtnTexPart,
										 leaderDownBtnTexPart);
		
		ret ->achBtn = new AnimButton(guidefAchievementsButton->GetAttributeInt("posx"),
									  guidefAchievementsButton->GetAttributeInt("posy"),
									  achBtnTexPart,
									  achDownBtnTexPart);
		
		ret ->m_changeNameBtn = new AnimButton(
				0,
				500,
				tpChangeButton,
				tpChangeButtonDown);

		ret->imgLogo = new Control(guidefLogo->GetAttributeInt("posx"),
								   guidefLogo->GetAttributeInt("posy"),
								   tpLogoPixel);
		
		ret->m_leaderboard = LeaderboardControl::Create();
		ret->m_leaderboard->SetPosition(0, 800);
		ret->AddChild(ret->m_leaderboard);

		ret ->optionsBtn->pushedShift.x += 6;
		ret ->optionsBtn->pushedShift.y += 4;

		ret ->AddChild(ret ->playGameBtn);
		ret ->AddChild(ret ->freePlayBtn);
		ret ->AddChild(ret ->optionsBtn);
		//ret ->AddChild(ret->leaderBtn);
		ret ->AddChild(ret->achBtn);
		ret ->AddChild(ret->m_changeNameBtn);
		ret->AddChild(ret->imgLogo);
		
		ObsCast(ITouchObserver, ret->playGameBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->freePlayBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->optionsBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->leaderBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->achBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->m_changeNameBtn)->AddObserver(ret);
	}
	
	Leaderboard::GetInstance()->AddObserver(ret);

	return ret;
}

void MainMenuPanel::TouchPressed(Control *control, int x, int y)
{
	if (control == playGameBtn)
		mmGameState->ItemSelected(MainMenuGameState::Menu_StartGame);
	else if (control == freePlayBtn)
		mmGameState->ItemSelected(MainMenuGameState::Menu_FreePlay);
	else if (control == optionsBtn)
		mmGameState->ItemSelected(MainMenuGameState::Menu_Options);
	else if (control == leaderBtn)
		mmGameState->ItemSelected(MainMenuGameState::Menu_Leaderboard);
	else if (control == achBtn)
		mmGameState->ItemSelected(MainMenuGameState::Menu_Achievements);
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
	leaderBtn->SetVisible(enabled);
	achBtn->SetVisible(enabled);
}

void MainMenuPanel::LeaderTopLoaded()
{
	m_leaderboard->SetPlayerStats(Leaderboard::GetInstance()->GetTopLadder());
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
