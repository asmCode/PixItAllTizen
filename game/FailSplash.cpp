#include "FailSplash.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include <math.h>
#include "Environment.h"

FailSplash::FailSplash()
{
	progress = 0.0f;
	tailProgress = 0.0f;
	bodySplashProgress = 0.0f;
	color = Color::White;
	color.A = 0;
	stopping = false;
	
	bodyTrans = sm::Matrix::ScaleMatrix(0, 0, 0);
	tailTrans = sm::Matrix::ScaleMatrix(0, 0, 0);
}

FailSplash *FailSplash::Create(SpriteBatch *sb)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	
	FailSplash *ret = new FailSplash();
	if (ret == NULL)
		return NULL;
		
	ret->texSplashBody = cc->GetClass<TexPart>("guimap_splash_body");
	ret->texSplashTail = cc->GetClass<TexPart>("guimap_splash_tail");
	ret->sb = sb;
	ret->xshift = Environment::GetInstance()->GetScreenWidth() / 2;
	
	return ret;
}

void FailSplash::DoSplash()
{
	progress = 0.0f;
	tailProgress = 0.0f;
	bodySplashProgress = 0.0f;
	color = Color::White;
	stopping = false;
	
	bodyTrans = sm::Matrix::ScaleMatrix(0, 0, 0);
	tailTrans = sm::Matrix::ScaleMatrix(0, 0, 0);
}

void FailSplash::UpdateTail(float time, float seconds)
{
	tailProgress += seconds * 1.2f;
	
	float tailSplashValue = tailProgress;
	
	tailTrans =
		sm::Matrix::TranslateMatrix(xshift + 8, 201, 0) *
		sm::Matrix::TranslateMatrix(0.0f, -0.45f, 0) *
		sm::Matrix::ScaleMatrix(texSplashTail.ImageRect.Width, texSplashTail.ImageRect.Height * tailSplashValue, 1) *
		sm::Matrix::TranslateMatrix(0, 0.45f, 0);
}

void FailSplash::UpdateBody(float time, float seconds)
{
	float bodySplashValue;
	//	float bodySplashValue = -4.0f * (bodySplashProgress - 0.5f) * (bodySplashProgress - 0.5f) + 1;
	//float xPart = bodySplashProgress - sqrt(0.5);
	//float bodySplashValue = -2.0 * xPart * xPart + 1;
	//printf("splash: f(%.4f) = %.4f\n", bodySplashProgress, bodySplashValue);
	//bodySplashValue = bodySplashProgress;
	bodySplashValue = -1.0f * (bodySplashProgress - 1.0f) * (bodySplashProgress -1.0f) + 1;	
	
	bodyTrans =
		sm::Matrix::TranslateMatrix(xshift, texSplashBody.ImageRect.Height / 2, 0) *
		sm::Matrix::ScaleMatrix(texSplashBody.ImageRect.Width * bodySplashValue, texSplashBody.ImageRect.Height * bodySplashValue, 1);
	
	bodySplashProgress += seconds * 5.0f;
	if (bodySplashProgress > 1.0f)
		bodySplashProgress = 1.0f;
}

void FailSplash::Update(float time, float ms)
{	
	if (color.A == 0)
		return;
	
	progress += ms;
	
	UpdateBody(time, ms);
	
	if (progress >= 0.2f)
		UpdateTail(time, ms);
	
	if (progress >= 1.8f || stopping)
		color.A = fmaxf(0, color.A - ms * 800.0f);

}

void FailSplash::Draw(float time, float ms)
{
	if (color.A == 0)
		return;
	
	sb->Draw(texSplashTail, color, tailTrans);
	sb->Draw(texSplashBody, color, bodyTrans);
}

bool FailSplash::IsActive() const
{
	return color.A != 0;
}

void FailSplash::StopSplash()
{
	stopping = true;
}

bool FailSplash::IsStopping() const
{
	return stopping;
}

