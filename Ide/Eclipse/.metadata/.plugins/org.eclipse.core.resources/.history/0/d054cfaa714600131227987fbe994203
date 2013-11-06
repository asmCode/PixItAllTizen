#include "Fade2.h"
#include <Graphics/SpriteBatch.h>
#include "Environment.h"
#include <stdio.h>

Fade2::Fade2()
{
	sb = new SpriteBatch();
	duration = 2.0f;
	fadeDirection = -1.0f;
	fadeValue = 0.0f;
	ready = true;
	
	resWidth = Environment::GetInstance()->GetScreenWidth();
	resHeight = Environment::GetInstance()->GetScreenHeight();
}

Fade2::~Fade2()
{
	if (sb != NULL)
		delete sb;
}

void Fade2::Update(float time, float seconds)
{
	if ((fadeValue == 0.0f && fadeDirection == -1.0f) ||
		(fadeValue == 1.0f && fadeDirection ==  1.0f))
	{
		ready = true;
		return;
	}
	
	fadeColor = Color(0, 0, 0, (unsigned char)(fadeValue * 255.0f));
	
	fadeValue += (seconds / duration) * fadeDirection;
	fadeValue = fminf(fmaxf(0.0f, fadeValue), 1.0f);
}

void Fade2::Draw(float time, float seconds)
{
	if (fadeValue == 0.0f)
		return;
	
	sb->Begin();
	sb->Draw(fadeColor, 0, 0, resWidth, resHeight);
	sb->End();
}

void Fade2::StartFadeIn(float duration)
{
	this->duration = duration;

	ready = false;
	fadeValue = 0.0f;
	fadeDirection = 1.0f;
}

void Fade2::StartFadeOut(float duration)
{
	this->duration = duration;

	ready = false;
	fadeValue = 1.0f;
	fadeDirection = -1.0f;
}

bool Fade2::IsReady() const
{
	return ready;
}