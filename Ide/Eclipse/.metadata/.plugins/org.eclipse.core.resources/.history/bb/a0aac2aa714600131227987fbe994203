#pragma once

#include <Graphics/IUpdateable.h>
#include <Graphics/IDrawable.h>
#include <Graphics/TexPart.h>
#include <Graphics/Color.h>
#include "ClassContainer.h"
#include <Utils/Math/Matrix.h>

class SpriteBatch;

class Splash :
public IUpdateable,
public IDrawable
{
public:
	enum SplashType
	{
		SplashType_Opacity,
		SplashType_Scale
	};
	
private:
	Splash();
	
	TexPart		tpSplash;
	SplashType	splashType;
	
	sm::Matrix	trans;
	float		opacity;
	
	SpriteBatch *sb;
	
	float progress;
	float fadeDuration;
	float stillDuration;
	float totalDuration;
	
public:
	Splash(SpriteBatch *sb);
	
	void DoSplash(float fadeDuration, float stillDuration, SplashType splashType, TexPart tpSplash);
	
	virtual void Update(float time, float ms);
	virtual void Draw(float time, float ms);
	virtual bool IsReady() const;
};
