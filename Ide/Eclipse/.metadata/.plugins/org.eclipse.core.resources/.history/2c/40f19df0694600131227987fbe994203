#pragma once

#include <string>
#include <Utils/GenericSingleton.h>
#include "IGameCommunity.h"
#include <Graphics/Point.h>

class AchievementsManager : public virtual GenericSingleton<AchievementsManager>
{
private:
	static const unsigned SmallLevel = 0;
	static const unsigned MediumLevel = 500;
	static const unsigned BigLevel = 1000;
	
	float lastTime;
	IGameCommunity *gc;
	
	unsigned	width;
	unsigned	height;
	unsigned	colorsCount;
	
	unsigned	inARow;
	unsigned	inARowNoPreview;
	unsigned	colorChangedCount;
	bool		horiMachine;
	bool		vertMachine;
	bool		noPreview;
	bool		superSpeed;
	bool		flawlessVictory;
	
	sm::Point<int> lastCorrectPixel;
	
public:
	void SetGameCommunity(IGameCommunity *gc);
	
	void UserRatedApp();
	void FinishedTutorial();
	void GameStarted(float time, unsigned width, unsigned height, unsigned colorsCount);
	void GameFinished(bool victory);
	void PickedCorrectColor(float time, int x, int y);
	void PickedWrongColor();
	void ChangedColor();
	void PreviewShown();
};
