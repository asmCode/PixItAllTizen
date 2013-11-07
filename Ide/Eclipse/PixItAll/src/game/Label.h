#pragma once

#include "Control.h"
#include <GraphicsLibrary/Color.h>
#include <string>

class FontRenderer;

class Label : public Control
{
private:
	std::string text;
	Color textColor;
	FontRenderer *fontRenderer;
	
public:
	Label(const std::string &text,
		  FontRenderer *fontRenderer,
		  const Color &textColor,
		  int x,
		  int y);
	
	void SetText(const std::string &text);
	const std::string& GetText();
	
	void OnUpdate(float time, float ms);
	void OnDraw(float time, float ms);
	
	sm::Point<int> GetTextSize();
};
