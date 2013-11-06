#pragma once

#include <string>
#include <GraphicsLibrary/IUpdateable.h>
#include "ObserverSubject.h"
#include "IPropertyObserver.h" 

class Player : public virtual IUpdateable, public ObserversContainer<IPropertyObserver*>
{
private:
	std::string name;
	
	unsigned failLimit;
	unsigned failCount;
	unsigned totalFailCount;
	unsigned combo;
	
	float credits;
	float timeElapsed;
	
public:
	static const float PreviewCoinsPerSecond;
	static const float PointsPerPixel;
	
	Player(unsigned failLimit);
	
	void Update(float time, float ms);
	
	void SetTime(float time);
	float GetTime() const;
	
	void AddCredits(float credits);
	float GetCredits() const;
	
	int CalcPoints(unsigned imgWidth,
				   unsigned imgHeight,
				   unsigned colorsCount) const;

	void AddFail();
	unsigned GetFailCount() const;
	unsigned GetTotalFailCount() const;
	unsigned GetFailLimit() const;
	void ResetFails();
};
