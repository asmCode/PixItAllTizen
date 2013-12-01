#include "LeaderboardControl.h"
#include "ClassContainer.h"
#include "Environment.h"
#include <GraphicsLibrary/TexPart.h>
#include "MainMenuGameState.h"
#include "MessageBox.h"
#include "SoundManager.h"
#include "XMLElement.h"
#include <stdio.h>

template<typename T> extern T* GenericSingleton<T>::instance;

LeaderboardControl::LeaderboardControl()
{
	playGameBtn = NULL;
}

LeaderboardControl *LeaderboardControl::Create()
{
	ClassContainer *cc = ClassContainer::GetInstance();

	FontRenderer *defaultFont = NULL;
	ClassContainer::GetInstance()->TryGetClass("defaultFont", defaultFont);

	LeaderboardControl *ret = new LeaderboardControl();
	if (ret != NULL)
	{
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = PixItAll::Environment::GetInstance()->GetScreenWidth();
		ret ->height = PixItAll::Environment::GetInstance()->GetScreenHeight();

		TexPart playGameBtnTexPart = cc ->GetClass<TexPart>("guimap_mm_play_game_btn");
		TexPart playGameDownBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_play_game_down_btn");

		ret->bg = cc ->GetClass<TexPart>("guimap_main_menu_bg");

		ret ->playGameBtn = new AnimButton(0, 0, playGameBtnTexPart, playGameDownBtnTexPart);

		//ret->imgLogo = new Control(0, 0, tpLogoPixel);

		ret->m_placeLabel = new Label("Place", defaultFont, Color::Red, PlaceColumnShift, 0);
		ret->m_playerNameLabel = new Label("Name", defaultFont, Color::Red, NameColumnShift, 0);
		ret->m_pointsLabel = new Label("Points", defaultFont, Color::Red, PointsColumnShift, 0);
		ret->m_levelsLabel = new Label("Levels", defaultFont, Color::Red, LevelColumnShift, 0);

		ret->AddChild(ret->m_placeLabel);
		ret->AddChild(ret->m_playerNameLabel);
		ret->AddChild(ret->m_pointsLabel);
		ret->AddChild(ret->m_levelsLabel);

		//ret->AddChild(ret ->playGameBtn);
		//ret->AddChild(ret->imgLogo);

		ObsCast(ITouchObserver, ret->playGameBtn) ->AddObserver(ret);
	}

	return ret;
}

void LeaderboardControl::TouchPressed(Control *control, int x, int y)
{
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
}

void LeaderboardControl::OnDraw(float time, float seconds)
{
	this->Control::OnDraw(time, seconds);
}

void LeaderboardControl::PropertyChanged(const std::string &propName, void *sender)
{
}

void LeaderboardControl::SetPlayerStats(const std::vector<PlayerStats>& playerStats)
{
	for (unsigned int i = 0; i < m_tableLabels.size(); i++)
	{
		RemoveChild(m_tableLabels[i]);
		delete m_tableLabels[i];
	}

	m_tableLabels.clear();

	FontRenderer *defaultFont = NULL;
	ClassContainer::GetInstance()->TryGetClass("smallFont", defaultFont);

	int yShift = 40;
	int yBase = 45;

	for (unsigned int i = 0; i < playerStats.size(); i++)
	{
		int yPos = yBase + i * yShift;

		char numberStr[32];

		sprintf(numberStr, "%d", playerStats[i].m_place);
		Label* placeLabel = new Label(numberStr, defaultFont, Color::Blue, PlaceColumnShift, yPos);

		Label* nameLabel = new Label(playerStats[i].m_name, defaultFont, Color::Blue, NameColumnShift, yPos);

		sprintf(numberStr, "%d", playerStats[i].m_points);
		Label* pointsLabel = new Label(numberStr, defaultFont, Color::Blue, PointsColumnShift, yPos);

		sprintf(numberStr, "%d", playerStats[i].m_levels);
		Label* levelsLabel = new Label(numberStr, defaultFont, Color::Blue, LevelColumnShift, yPos);

		AddChild(placeLabel);
		AddChild(nameLabel);
		AddChild(pointsLabel);
		AddChild(levelsLabel);

		m_tableLabels.push_back(placeLabel);
		m_tableLabels.push_back(nameLabel);
		m_tableLabels.push_back(pointsLabel);
		m_tableLabels.push_back(levelsLabel);
	}
}
