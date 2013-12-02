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
}

LeaderboardControl *LeaderboardControl::Create()
{
	ClassContainer *cc = ClassContainer::GetInstance();

	FontRenderer *defaultFont = NULL;
	ClassContainer::GetInstance()->TryGetClass("defaultFont", defaultFont);

	LeaderboardControl *ret = new LeaderboardControl();
	if (ret != NULL)
	{
		TexPart playGameBtnTexPart = cc ->GetClass<TexPart>("guimap_mm_play_game_btn");
		TexPart playGameDownBtnTexPart =  cc->GetClass<TexPart>("guimap_mm_play_game_down_btn");

		ret->bg = cc ->GetClass<TexPart>("guimap_leaderboard");

		ret ->width = ret->bg.ImageRect.Width;
		ret ->height = ret->bg.ImageRect.Height;
		ret ->x = 26;
		ret ->y = PixItAll::Environment::GetInstance()->GetScreenHeight() - ret ->height - 22;

		//ret ->playGameBtn = new AnimButton(0, 0, playGameBtnTexPart, playGameDownBtnTexPart);

		//ret->imgLogo = new Control(0, 0, tpLogoPixel);

		//ret->AddChild(ret ->playGameBtn);
		//ret->AddChild(ret->imgLogo);

		//ObsCast(ITouchObserver, ret->playGameBtn) ->AddObserver(ret);
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

	FontRenderer *allan18Font = NULL;
	ClassContainer::GetInstance()->TryGetClass("allan18Font", allan18Font);

	int yBase = 110;

	for (unsigned int i = 0; i < playerStats.size(); i++)
	{
		int yPos = yBase + i * RowHeight;

		char numberStr[32];

		sprintf(numberStr, "%d", playerStats[i].m_place);
		Label* placeLabel = new Label(numberStr, allan18Font, Color::White, PlaceColumnShift, yPos);

		Label* nameLabel = new Label(playerStats[i].m_name, allan18Font, Color::White, NameColumnShift, yPos);

		sprintf(numberStr, "%d", playerStats[i].m_points);
		Label* pointsLabel = new Label(numberStr, allan18Font, Color::White, PointsColumnShift, yPos);

		sprintf(numberStr, "%d", playerStats[i].m_levels);
		Label* levelsLabel = new Label(numberStr, allan18Font, Color::White, LevelColumnShift, yPos);

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
