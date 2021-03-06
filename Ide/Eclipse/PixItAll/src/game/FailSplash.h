#pragma once

#include <GraphicsLibrary/IUpdateable.h>
#include <GraphicsLibrary/IDrawable.h>
#include <GraphicsLibrary/TexPart.h>
#include <GraphicsLibrary/Color.h>
#include "ClassContainer.h"
#include <Utils/Math/Matrix.h>

class SpriteBatch;

class FailSplash :
	public IUpdateable,
	public IDrawable
{
private:
	FailSplash();
	
	TexPart texSplashBody;
	TexPart texSplashTail;
	
	sm::Matrix bodyTrans;
	sm::Matrix tailTrans;
	Color color;
	
	float xshift;
	
	SpriteBatch *sb;
	
	float progress;
	float tailProgress;
	float bodySplashProgress;
	bool stopping;
	
	void UpdateTail(float time, float seconds);
	void UpdateBody(float time, float seconds);	
	
public:
	static FailSplash *Create(SpriteBatch *sb);
	
	void DoSplash();
	
	void Update(float time, float ms);
	void Draw(float time, float ms);
	bool IsActive() const;
	
	void StopSplash();
	bool IsStopping() const;
};
