#include "BonusSplash.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include <math.h>
#include "ParticleManager.h"
#include "Environment.h"

const float BonusSplash::ScaleTime = 5.0f;
const float BonusSplash::FadeTime = 2.0f;
const float BonusSplash::ParticlesDuration = 3.0f;

BonusSplash::BonusSplash()
{
	progress = 0.0f;
	color = Color::White;
	color.A = 0;
	
	trans = sm::Matrix::ScaleMatrix(0, 0, 0);
	partMng = NULL;
}

BonusSplash::~BonusSplash()
{
	if (partMng != NULL)
		delete partMng;
}

BonusSplash *BonusSplash::Create(SpriteBatch *sb)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	
	BonusSplash *ret = new BonusSplash();
	if (ret == NULL)
		return NULL;
	
	ret ->tpBonus5 = cc->GetClass<TexPart>("guimap_bonus_5_row");
	ret ->tpBonus10 = cc->GetClass<TexPart>("guimap_bonus_10_row");
	ret ->tpBonus20 = cc->GetClass<TexPart>("guimap_bonus_20_row");
	ret ->sb = sb;
	
	float resWidth = PixItAll::Environment::GetInstance()->GetScreenWidth();
	float resHeight = PixItAll::Environment::GetInstance()->GetScreenHeight();
	
	ret->partMng = new ParticleManager();
	ret->partMng->Initialize(20, sm::Vec2(resWidth / 2, resHeight * 0.2f));
	
	ret->xshift = resWidth / 2;
	ret->yshift = resHeight * 0.2f;
	
	return ret;
} 

void BonusSplash::DoSplash(BonusSplash::BonusType bonusType)
{
	switch (bonusType)
	{
		case BonusType_5InARow:
			tpCurrentBonus = tpBonus5;
			break;
			
		case BonusType_10InARow:
			tpCurrentBonus = tpBonus10;
			break;
			
		case BonusType_20InARow:
			tpCurrentBonus = tpBonus20;
			break;
	}
	
	progress = 0.0f;
	color = Color::White;
	
	trans = sm::Matrix::ScaleMatrix(0, 0, 0);
	partMng->Blow(ParticlesDuration, 200.0f, 0.5f);
}

void BonusSplash::Update(float time, float ms)
{	
	if (partMng->IsActive())
		partMng->Update(time, ms);
	
	if (color.A == 0)
		return;
	
	progress += ms;
	
	if (progress >= 1.0f && color.A > 0)
	{
		color.A = (unsigned char)fmaxf(((1.0f - (progress - 1.0f) / 1.0f) * 255.0f), 0.0f);
	}
	
	float splashProgres = fminf(1.0f, progress * 2);
	
	float bodySplashValue = -1.0f * (splashProgres - 1.0f) * (splashProgres -1.0f) + 1;	
	
	trans =
		sm::Matrix::TranslateMatrix(xshift, yshift, 0) *
		sm::Matrix::ScaleMatrix(tpCurrentBonus.ImageRect.Width * bodySplashValue, tpCurrentBonus.ImageRect.Height * bodySplashValue, 1);
}

void BonusSplash::Draw(float time, float ms)
{
	if (partMng->IsActive())
		partMng->Draw(time, ms);
	
	if (color.A == 0)
		return;

	sb->Draw(tpCurrentBonus, color, trans);
}

bool BonusSplash::IsActive() const
{
	return color.A != 0;
}

bool BonusSplash::IsReady() const
{
	return progress == 3.0f;
}
