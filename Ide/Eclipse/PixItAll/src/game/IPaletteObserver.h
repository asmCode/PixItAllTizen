#pragma once

#include <GraphicsLibrary/Color.h>

class IPaletteObserver
{
public:
	virtual void PaletteColorChanged(const Color &color) {};
};
