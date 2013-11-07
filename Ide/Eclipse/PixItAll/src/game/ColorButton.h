#pragma once

#include "Control.h"
#include <GraphicsLibrary/Color.h>

class ColorButton : public Control
{
private:
	Color color;
	void OnDraw(float time, float ms);
	
public:
	ColorButton(int x, int y, const TexPart &bg, const Color &color);
	void SetColor(const Color &color);
};
