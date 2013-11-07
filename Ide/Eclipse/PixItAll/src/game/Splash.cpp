#include "Splash.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include <math.h>
#include "Environment.h"

Splash::Splash(SpriteBatch *sb)
{
	this->sb = sb;
	progress = 0.0f;
	totalDuration = 0.0f;
}

void Splash::DoSplash(float fadeDuration, float stillDuration, SplashType splashType, TexPart tpSplash)
{
	opacity = 1.0f;
	this->tpSplash = tpSplash;
	this->splashType = splashType;
	this->fadeDuration = fadeDuration;
	this->stillDuration = stillDuration;
	
	totalDuration = 2.0f * fadeDuration + stillDuration;
	progress = 0.0f;
	
	
	trans = sm::Matrix::ScaleMatrix(0, 0, 0);
}

void Splash::Update(float time, float ms)
{	
	if (IsReady())
		return;
	
	float xshift = PixItAll::Environment::GetInstance()->GetScreenWidth() / 2;
	float yshift = PixItAll::Environment::GetInstance()->GetScreenHeight() / 3;
	
	progress += ms;
	progress = fminf(totalDuration, progress);
	
	float subProgress = 0.0f;
		
	trans =	sm::Matrix::TranslateMatrix(xshift, yshift, 0);
	
	switch (splashType)
	{
		case SplashType_Scale:
			if (progress >= fadeDuration + stillDuration)
			{
				subProgress = (progress - fadeDuration - stillDuration) / fadeDuration;
				
				opacity = 1.0f - subProgress;
				trans =
					trans *
					sm::Matrix::ScaleMatrix(tpSplash.ImageRect.Width, tpSplash.ImageRect.Height, 1);

			}
			else if (progress >= fadeDuration)
			{
				subProgress = (progress - fadeDuration) / stillDuration;
				
				opacity = 1.0f;
				trans =
					trans *
					sm::Matrix::ScaleMatrix(tpSplash.ImageRect.Width, tpSplash.ImageRect.Height, 1);
				
			}
			else
			{
				subProgress = progress / fadeDuration;
				subProgress = -1.0f * (subProgress - 1.0f) * (subProgress -1.0f) + 1;	
				
				opacity = 1.0f;
				trans =
					trans *
					sm::Matrix::ScaleMatrix(tpSplash.ImageRect.Width * subProgress, tpSplash.ImageRect.Height * subProgress, 1);
			}
			
			break;
			
		case SplashType_Opacity:
			trans =
				trans *
				sm::Matrix::ScaleMatrix(tpSplash.ImageRect.Width, tpSplash.ImageRect.Height, 1);

			if (progress >= fadeDuration + stillDuration)
			{
				subProgress = (progress - fadeDuration - stillDuration) / fadeDuration;
				opacity = 1.0f - subProgress;
			}
			else if (progress >= fadeDuration)
			{
				subProgress = (progress - fadeDuration) / stillDuration;
				opacity = 1.0f;
			}
			else
			{
				subProgress = progress / fadeDuration;
				opacity = subProgress;
			}

			break;
	}
}

void Splash::Draw(float time, float ms)
{
	if (IsReady())
		return;
	
	sb->Begin();
	sb->Draw(tpSplash, Color(255, 255, 255, (unsigned char)(opacity * 255.0f)), trans);
	sb->End();
}

bool Splash::IsReady() const
{
	return progress >= totalDuration;
}
