#include "Player.h"

const float Player::PreviewCoinsPerSecond = 12.0f;
const float Player::PointsPerPixel = 1.0f;

Player::Player(unsigned failLimit)
{
	this->failLimit = failLimit;
	failCount = 0;
	totalFailCount = 0;
	combo = 0;
	
	credits = 0;
	timeElapsed = 0;
}

void Player::Update(float time, float ms)
{
	SetTime(timeElapsed + ms);
}

void Player::SetTime(float time)
{
	timeElapsed = time;
	
	ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, "timeElapsed", this);
}

float Player::GetTime() const
{
	return timeElapsed;
}

void Player::AddCredits(float credits)
{
	float prevVal = this->credits;
	
	this->credits += credits;
	if (this->credits < 0)
		this->credits = 0;
	
	if (prevVal != this->credits)
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, "credits", this);
}

float Player::GetCredits() const
{
	return credits;
}

int Player::CalcPoints(unsigned imgWidth,
					   unsigned imgHeight,
					   unsigned colorsCount) const
{
	int totalPoints = (int)((float)(imgWidth * imgHeight * colorsCount) / (timeElapsed * 0.005f) + (credits - (failCount * 10)));
	if (totalPoints < 1)
		totalPoints = 1;
	return totalPoints;
}

void Player::AddFail()
{
	totalFailCount++;
	failCount++;
}

unsigned Player::GetTotalFailCount() const
{
	return totalFailCount;
}

unsigned Player::GetFailLimit() const
{
	return failLimit;
}

unsigned Player::GetFailCount() const
{
	return failCount;
}

void Player::ResetFails()
{
	failCount = 0;
}
