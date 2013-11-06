#pragma once

#include <Graphics/IUpdateable.h>
#include <Graphics/IDrawable.h>
#include <Graphics/TexPart.h>
#include <Graphics/Color.h>
#include "ClassContainer.h"
#include <Utils/Math/Matrix.h>

class SpriteBatch;
class ParticleManager;

class BonusSplash :
public IUpdateable,
public IDrawable
{
public:
	enum BonusType
	{
		BonusType_5InARow,
		BonusType_10InARow,
		BonusType_20InARow
	};
	
private:
	static const float ScaleTime;
	static const float FadeTime;
	static const float ParticlesDuration;
	
	BonusSplash();
	
	ParticleManager *partMng;
	
	TexPart tpBonus5;
	TexPart tpBonus10;
	TexPart tpBonus20;
	
	float xshift;
	float yshift;
	
	TexPart tpCurrentBonus;
	
	sm::Matrix trans;
	Color color;
	
	SpriteBatch *sb;
	
	float progress;
	
public:
	static BonusSplash *Create(SpriteBatch *sb);
	
	~BonusSplash();
	
	void DoSplash(BonusType bonusType);
	
	void Update(float time, float ms);
	void Draw(float time, float ms);
	bool IsActive() const;
	bool IsReady() const;
};
