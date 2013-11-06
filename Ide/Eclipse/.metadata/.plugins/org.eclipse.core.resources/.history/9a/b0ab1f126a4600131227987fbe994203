#include "Label.h"
#include <Graphics/FontRenderer.h>

Label::Label(const std::string &text,
			 FontRenderer *fontRenderer,
			 const Color &textColor,
			 int x,
			 int y) :
	Control(x, y, 0, 0)
{
	this ->text = text;
	this ->fontRenderer = fontRenderer;
	this ->textColor = textColor;
	this->opacity = 1.0f;
}

void Label::OnUpdate(float time, float ms)
{
}

void Label::OnDraw(float time, float ms)
{
	Color color = textColor;
	if (parent != NULL)
		color.A = (unsigned char)((float)color.A * parent->GetOpacity() * opacity);
	sm::Vec2 globalPos = GetGlobalPos();
	fontRenderer ->DrawString(text.c_str(), globalPos.x, globalPos.y, color);
}

void Label::SetText(const std::string &text)
{
	this ->text = text;
}

const std::string& Label::GetText()
{
	return this ->text;
}

sm::Point<int> Label::GetTextSize()
{
	return fontRenderer->MeasureString(text.c_str());
}

