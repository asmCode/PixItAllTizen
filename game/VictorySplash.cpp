#include "VictorySplash.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include <math.h>

const float VictorySplash::Duration = 4.0f;

VictorySplash::VictorySplash(SpriteBatch *sb) : Splash(sb)
{
	victorySplashTime = Duration;
}

VictorySplash *VictorySplash::Create(SpriteBatch *sb)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	
	VictorySplash *ret = new VictorySplash(sb);
	if (ret == NULL)
		return NULL;
	
	ret ->tpVictory = cc->GetClass<TexPart>("guimap_victory");
	
	return ret;
}

void VictorySplash::DoSplash()
{
	victorySplashTime = 0.0f;
	Splash::DoSplash(0.5f, 1.0f, Splash::SplashType_Scale, tpVictory);
}

void VictorySplash::Update(float time, float ms)
{	
	this->Splash::Update(time, ms);
	
	victorySplashTime += ms;
}

void VictorySplash::Draw(float time, float ms)
{
	this->Splash::Draw(time, ms);
}

bool VictorySplash::IsActive() const
{
	return victorySplashTime < Duration;
}

bool VictorySplash::IsReady() const
{
	return victorySplashTime >= Duration;
}

void VictorySplash::Cancel()
{
	victorySplashTime = Duration;
}
