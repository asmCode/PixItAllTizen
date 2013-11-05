#include "AchievementsManager.h"
#include "AchievementsNames.h"

void AchievementsManager::SetGameCommunity(IGameCommunity *gc)
{
	this->gc = gc;
}

void AchievementsManager::UserRatedApp()
{
	gc->ReportAchievement(AchievementsNames::FriendlyPlayer);
}

void AchievementsManager::FinishedTutorial()
{
	gc->ReportAchievement(AchievementsNames::HomeWork);
}
 
void AchievementsManager::GameStarted(float time, unsigned width, unsigned height, unsigned colorsCount)
{
	this->lastTime = time;
	this->width = width;
	this->height = height;
	this->colorsCount = colorsCount;

	inARow = 0;
	inARowNoPreview = 0;
	colorChangedCount = 0;
	horiMachine = true;
	vertMachine = true;
	noPreview = true;
	superSpeed = true;
	flawlessVictory = true;
	
	lastCorrectPixel = sm::Point<int>(-1, -1);
}

void AchievementsManager::GameFinished(bool victory)
{
	if (horiMachine && victory)
		gc->ReportAchievement(AchievementsNames::HorizontalMachine);
	
	if (vertMachine && victory)
		gc->ReportAchievement(AchievementsNames::VerticalMachine);
	
	if (flawlessVictory && victory)
	{
		if (width * height >= BigLevel)
			gc->ReportAchievement(AchievementsNames::FlawlessVictoryBig);
		else if (width * height >= MediumLevel)
			gc->ReportAchievement(AchievementsNames::FlawlessVictoryMedium);
		else
			gc->ReportAchievement(AchievementsNames::FlawlessVictorySmall);
	}
	
	if (noPreview && victory)
	{
		if (width * height >= BigLevel)
			gc->ReportAchievement(AchievementsNames::TotalRecallBig);
		else if (width * height >= MediumLevel)
			gc->ReportAchievement(AchievementsNames::TotalRecallMedium);
		else
			gc->ReportAchievement(AchievementsNames::TotalRecallSmall);
	}
	
	if (superSpeed && victory)
	{
		if (width * height >= BigLevel)
			gc->ReportAchievement(AchievementsNames::SuperSpeedBig);
		else if (width * height >= MediumLevel)
			gc->ReportAchievement(AchievementsNames::SuperSpeedMedium);
		else
			gc->ReportAchievement(AchievementsNames::SuperSpeedSmall);
	}

	
	if (colorChangedCount <= colorsCount && victory)
		gc->ReportAchievement(AchievementsNames::TakeOnlyWhatYouNeedFromIt);
}

void AchievementsManager::PickedCorrectColor(float time, int x, int y)
{
	if (time - lastTime > 4.0f)
		superSpeed = false;
	
	lastTime = time;
	
	inARow++;
	inARowNoPreview++;
	
	if (lastCorrectPixel.X == -1 && lastCorrectPixel.Y == -1)
	{
		lastCorrectPixel = sm::Point<int>(x, y);
	}
	else
	{
		if ((y < lastCorrectPixel.Y) ||
			(x < lastCorrectPixel.X && y <= lastCorrectPixel.Y))
			horiMachine = false;
		
		if ((x < lastCorrectPixel.X) ||
			(y < lastCorrectPixel.Y && x <= lastCorrectPixel.X))
			vertMachine = false;
	}
	
	lastCorrectPixel = sm::Point<int>(x, y);
	
	if (inARow == 50)
		gc->ReportAchievement(AchievementsNames::InARow50);
	if (inARow == 100)
		gc->ReportAchievement(AchievementsNames::InARow100);
	if (inARow == 150)
		gc->ReportAchievement(AchievementsNames::InARow150);
	if (inARow == 200)
		gc->ReportAchievement(AchievementsNames::InARow200);
	
	if (inARowNoPreview == 50)
		gc->ReportAchievement(AchievementsNames::InARowNoPreview50);
	if (inARowNoPreview == 100)
		gc->ReportAchievement(AchievementsNames::InARowNoPreview100);
	if (inARowNoPreview == 150)
		gc->ReportAchievement(AchievementsNames::InARowNoPreview150);
	if (inARowNoPreview == 200)
		gc->ReportAchievement(AchievementsNames::InARowNoPreview200);
}

void AchievementsManager::PickedWrongColor()
{
	inARowNoPreview = 0;
	inARow = 0;
	flawlessVictory = false;
}

void AchievementsManager::ChangedColor()
{
	colorChangedCount++;
}

void AchievementsManager::PreviewShown()
{
	noPreview = false;
	inARowNoPreview = 0;
}
