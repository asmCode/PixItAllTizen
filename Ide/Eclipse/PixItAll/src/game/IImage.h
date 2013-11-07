#pragma once

#include <GraphicsLibrary/Color.h>

class IImage
{
public:
	virtual ~IImage(){}
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual Color GetColor(int i, int y) const = 0;
};
