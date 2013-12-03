#include "AnimButton.h"
#include <GraphicsLibrary/SpriteBatch.h>

AnimButton::AnimButton(float x, float y,
					   TexPart normalState,
					   TexPart pushedState,
					   TexPart shadow)
{
	m_checked = false;
	this->x = x;
	this->y = y;
	this->width = normalState.ImageRect.Width;
	this->height = normalState.ImageRect.Height;
	pushedShift = sm::Vec2(0, 0);
	currentShift = sm::Vec2(0, 0); 
	
	this->normalState = normalState;
	this->bg = normalState;
	this->pushedState = pushedState;
	this->shadow = shadow;
}

AnimButton::AnimButton(float x, float y,
					   TexPart normalState,
					   TexPart pushedState)
{
	this->x = x;
	this->y = y;
	this->width = normalState.ImageRect.Width;
	this->height = normalState.ImageRect.Height;
	pushedShift = sm::Vec2(0, 0);
	currentShift = sm::Vec2(0, 0); 
	
	this->normalState = normalState;
	this->bg = normalState;
	this->pushedState = pushedState;
	this->shadow = TexPart();
}

void AnimButton::OnDraw(float time, float ms)
{
	sm::Vec2 globalPos = GetGlobalPos();
	
	if (shadow.Tex != NULL)
		spriteBatch ->Draw(shadow, (int)globalPos.x + 5, (int)globalPos.y + 10);

	if (m_checked)
		spriteBatch ->Draw(pushedState, (int)globalPos.x + currentShift.x, (int)globalPos.y + currentShift.y);
	else
		spriteBatch ->Draw(bg, (int)globalPos.x + currentShift.x, (int)globalPos.y + currentShift.y);
}

void AnimButton::OnTouchBegin(int x, int y)
{
	bg = pushedState;
	currentShift = pushedShift;
}

void AnimButton::OnTouchEnd(int x, int y)
{
	bg = normalState;
	currentShift = sm::Vec2(0, 0);
}

void AnimButton::SetChecked(bool checked)
{
	m_checked = checked;
}
