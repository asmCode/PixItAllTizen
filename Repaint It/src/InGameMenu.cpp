#include "InGameMenu.h"
#include "ClassContainer.h"
#include "Environment.h"
#include <Graphics/TexPart.h>
#include "PlayGameState.h"
#include <Graphics/SpriteBatch.h>
#include "ToolsPanel.h"
#include <Utils/MemUtils.h>
#include "XMLElement.h"
#include "SoundManager.h"

InGameMenu::InGameMenu()
{
	btnBack = NULL;
	btnPlayAgain = NULL;
	btnMainMenu = NULL;
	btnOptions = NULL;
	btnPause = NULL;
	playGameState = NULL;
	isActive = false;
	
	resWidth = Environment::GetInstance()->GetScreenWidth();
	resHeight = Environment::GetInstance()->GetScreenHeight();
}

InGameMenu::~InGameMenu()
{
	MemUtils::DeleteObject(btnBack);
	MemUtils::DeleteObject(btnPlayAgain);
	MemUtils::DeleteObject(btnMainMenu);
	MemUtils::DeleteObject(btnOptions);
	MemUtils::DeleteObject(btnPause);
}

InGameMenu *InGameMenu::Create(PlayGameState *playGameState,
							   ToolsPanel *toolsPanel)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	
	InGameMenu *ret = new InGameMenu();
	if (ret != NULL)
	{
		ret->playGameState = playGameState;
		ret->toolsPanel = toolsPanel;
		
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = ret->resWidth;
		ret ->height = ret->resHeight;
		
		TexPart backTexPart = cc->GetClass<TexPart>("guimap_return_to_game_btn");
		TexPart backDownTexPart = cc->GetClass<TexPart>("guimap_return_to_game_down_btn");
		TexPart playAgainTexPart = cc->GetClass<TexPart>("guimap_play_again_btn");
		TexPart playAgainDownTexPart = cc->GetClass<TexPart>("guimap_play_again_down_btn");
		TexPart optionsTexPart = cc->GetClass<TexPart>("guimap_options_btn");
		TexPart optionsDownTexPart = cc->GetClass<TexPart>("guimap_options_down_btn");
		TexPart mainMenuTexPart = cc->GetClass<TexPart>("guimap_back_to_menu_btn");
		TexPart mainMenuDownTexPart = cc->GetClass<TexPart>("guimap_back_to_menu_down_btn");
		
		XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_InGameMenu");
		XMLElement *guidefPause = guidefPanel->GetSubElement(0);
		
		float leftColumn = (float)ret->resWidth * (2.0f / 6.0f) - (backTexPart.ImageRect.Width / 2);
		float rightColumn = (float)ret->resWidth * (4.0f / 6.0f) - (backTexPart.ImageRect.Width / 2);
		float vertStep = (float)ret->resHeight * (1.0f / 8.0f);
		float vertShift = backTexPart.ImageRect.Height / 2 + 20;
		
		ret ->btnBack = new AnimButton(leftColumn, vertShift, backTexPart, backDownTexPart);
		ret ->btnPlayAgain = new AnimButton(rightColumn, vertShift += vertStep, playAgainTexPart, playAgainDownTexPart);
		ret ->btnOptions = new AnimButton(leftColumn, vertShift += vertStep, optionsTexPart, optionsDownTexPart);
		ret ->btnMainMenu = new AnimButton(rightColumn, vertShift += vertStep, mainMenuTexPart, mainMenuDownTexPart);
		ret ->btnPause = new AnimButton(guidefPause->GetAttributeInt("posx"),
										guidefPause->GetAttributeInt("posy"),
										cc->GetClass<TexPart>("guimap_pause"),
										cc->GetClass<TexPart>("guimap_pause_down"));
				
		ret ->AddChild(ret ->btnBack);
		ret ->AddChild(ret ->btnPlayAgain);
		ret ->AddChild(ret ->btnOptions);
		ret ->AddChild(ret ->btnMainMenu);
		ret ->AddChild(ret ->btnPause);
		
		ObsCast(ITouchObserver, ret ->btnBack) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret ->btnPlayAgain) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret ->btnOptions) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret ->btnMainMenu) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret ->btnPause) ->AddObserver(ret);
	}
	
	return ret;
}

bool InGameMenu::IsActive() const
{
	return isActive;
}

void InGameMenu::SetActive(bool active)
{
	isActive = active;
	
	toolsPanel->ShowPauseButton(!isActive);
}

void InGameMenu::TouchPressed(Control *control, int x, int y)
{
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
	
	if (control == btnBack || control == btnPause)
		SetActive(false);
	else if (control == btnPlayAgain)
		playGameState->PlayAgain();
	else if (control == btnOptions)
		playGameState->ShowOptions();
	else if (control == btnMainMenu)
		playGameState->BackToMenu();
}

void InGameMenu::OnDraw(float time, float seconds)
{
	spriteBatch->Draw(Color(80, 80, 80, 160), 0, 0, resWidth, resHeight);
}
