#pragma once

#include <GraphicsLibrary/IUpdateable.h>
#include <GraphicsLibrary/IDrawable.h>
#include <GraphicsLibrary/TexPart.h>
#include <GraphicsLibrary/Color.h>
#include "ClassContainer.h"
#include <Utils/Math/Matrix.h>
#include "Splash.h"

class SpriteBatch;

class VictorySplash : public Splash
{
private:
	static const float Duration;
	
	VictorySplash(SpriteBatch *sb);
	
	TexPart tpVictory;
	
	float victorySplashTime;
		
public:
	static VictorySplash *Create(SpriteBatch *sb);
	
	void DoSplash();
	
	void Update(float time, float ms);
	void Draw(float time, float ms);
	bool IsActive() const;
	bool IsReady() const;
	
	void Cancel();
};
