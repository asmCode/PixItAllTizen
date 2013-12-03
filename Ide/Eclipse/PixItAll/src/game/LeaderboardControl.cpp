#include "LeaderboardControl.h"
#include "ClassContainer.h"
#include "Environment.h"
#include <GraphicsLibrary/TexPart.h>
#include "MainMenuGameState.h"
#include "MessageBox.h"
#include "Leaderboard.h"
#include "ImagesCollection.h"
#include "SoundManager.h"
#include "XMLElement.h"
#include "PlayerData.h"
#include <stdio.h>

template<typename T> extern T* GenericSingleton<T>::instance;

LeaderboardControl::LeaderboardControl()
{
	m_currentTab = Tab_Top;
}

LeaderboardControl *LeaderboardControl::Create()
{
	ClassContainer *cc = ClassContainer::GetInstance();

	FontRenderer *defaultFont = NULL;
	ClassContainer::GetInstance()->TryGetClass("defaultFont", defaultFont);

	LeaderboardControl *ret = new LeaderboardControl();
	if (ret != NULL)
	{
		ret->bg = cc ->GetClass<TexPart>("guimap_leaderboard");

		TexPart tpOfflineMask = cc->GetClass<TexPart>("guimap_leaderboard_offline_mask");
		TexPart tpRetryBtn = cc->GetClass<TexPart>("guimap_retry_btn");
		TexPart tpRetryBtnDown= cc->GetClass<TexPart>("guimap_retry_btn_down");

		ret ->width = ret->bg.ImageRect.Width;
		ret ->height = ret->bg.ImageRect.Height;
		ret ->x = 26;
		ret ->y = PixItAll::Environment::GetInstance()->GetScreenHeight() - ret ->height - 22;

		ret->m_offlineMask = new Control(0, 0, ret->bg.ImageRect.Width, ret->bg.ImageRect.Height, tpOfflineMask);
		ret->m_retryButton = new AnimButton((720 - tpRetryBtn.ImageRect.Width) / 2, 300, tpRetryBtn, tpRetryBtnDown);
		ret->m_offlineLabel = new Label("You are offline", defaultFont, Color::White, 0, 230);
		ret->m_offlineLabel->SetX((720 - ret->m_offlineLabel->GetTextSize().X) / 2);

		ret->AddChild(ret->m_offlineMask);
		ret->AddChild(ret->m_retryButton);
		ret->AddChild(ret->m_offlineLabel);

		ObsCast(ITouchObserver, ret ->m_retryButton) ->AddObserver(ret);
	}

	return ret;
}

void LeaderboardControl::TouchPressed(Control *control, int x, int y)
{
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);

	if (control == m_retryButton)
		RefreshCurrentView();
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
	ClearTable();

	FontRenderer *allan18Font = NULL;
	ClassContainer::GetInstance()->TryGetClass("allan18Font", allan18Font);

	int yBase = 110;

	for (unsigned int i = 0; i < playerStats.size(); i++)
	{
		Color color = Color::White;

		if (playerStats[i].m_id != 0 &&
			playerStats[i].m_id == PlayerData::GetInstance()->m_id)
			color = Color(234, 60, 7);

		int yPos = yBase + i * RowHeight;

		char numberStr[32];

		sprintf(numberStr, "%d", playerStats[i].m_place);
		Label* placeLabel = new Label(numberStr, allan18Font, color, PlaceColumnShift, yPos);

		Label* nameLabel = new Label(playerStats[i].m_name, allan18Font, color, NameColumnShift, yPos);

		sprintf(numberStr, "%d", playerStats[i].m_points);
		Label* pointsLabel = new Label(numberStr, allan18Font, color, PointsColumnShift, yPos);

		sprintf(numberStr, "%d", playerStats[i].m_levels);
		Label* levelsLabel = new Label(numberStr, allan18Font, color, LevelColumnShift, yPos);

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

void LeaderboardControl::SetTab(LeaderboardControl::Tab tab)
{
	m_currentTab = tab;
}

void LeaderboardControl::RefreshCurrentView()
{
	if (m_currentTab == Tab_Top)
		Leaderboard::GetInstance()->RefreshTopLadder();
	else if (m_currentTab == Tab_You)
			Leaderboard::GetInstance()->RefreshSurrLadder(ImagesCollection::Instance->GetTotalPoints());
}

void LeaderboardControl::SetOffline()
{
	ClearTable();

	m_offlineMask->SetVisible(true);
	m_offlineLabel->SetVisible(true);
	m_retryButton->SetVisible(true);
}

void LeaderboardControl::SetOnline()
{
	m_offlineMask->SetVisible(false);
	m_offlineLabel->SetVisible(false);
	m_retryButton->SetVisible(false);
}

void LeaderboardControl::ClearTable()
{
	for (unsigned int i = 0; i < m_tableLabels.size(); i++)
	{
		RemoveChild(m_tableLabels[i]);
		delete m_tableLabels[i];
	}

	m_tableLabels.clear();
}
