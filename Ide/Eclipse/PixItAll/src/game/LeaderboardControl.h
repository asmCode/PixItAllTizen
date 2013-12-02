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
	static const int RowHeight = 71;
	static const int PlaceColumnShift = 20;
	static const int NameColumnShift = 130;
	static const int PointsColumnShift = 390;
	static const int LevelColumnShift = 560;

	AnimButton *playGameBtn;

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
