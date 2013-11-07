#include "ColorButton.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include <GraphicsLibrary/Color.h>
	
ColorButton::ColorButton(int x, int y, const TexPart &bg, const Color &color) :
Control(x, y, bg), color(color)
{
	this->color = color;
}

void ColorButton::OnDraw(float time, float ms)
{
	sm::Vec2 globalPos = GetGlobalPos();
	spriteBatch ->Draw(bg, color, (int)globalPos.x, (int)globalPos.y, width, height);
}

void ColorButton::SetColor(const Color &color)
{
	this->color = color;
}
