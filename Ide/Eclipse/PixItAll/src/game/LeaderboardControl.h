#pragma once

#include "ITouchObserver.h"
#include "IPropertyObserver.h"
#include "Control.h"
#include "Label.h"
#include "AnimButton.h"
#include "PlayerStats.h"
#include <vector>

class LeaderboardControl :
	public Control,
	public ITouchObserver,
	public IPropertyObserver
{
private:
	static const int PlaceColumnShift = 0;
	static const int NameColumnShift = 160;
	static const int PointsColumnShift = 400;
	static const int LevelColumnShift = 600;

	AnimButton *playGameBtn;
	Label* m_placeLabel;
	Label* m_playerNameLabel;
	Label* m_pointsLabel;
	Label* m_levelsLabel;

	Control *imgLogo;

	std::vector<Label*> m_tableLabels;

	LeaderboardControl();

	void PropertyChanged(const std::string &propName, void *sender);
	void TouchPressed(Control *control, int x, int y);
	void OnDraw(float time, float seconds);

public:
	static LeaderboardControl *Create();

	void SetPlayerStats(const std::vector<PlayerStats>& playerStats);
};
