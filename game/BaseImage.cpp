#include "BaseImage.h"
#include <Utils/MemUtils.h>
#include <stddef.h>

BaseImage::BaseImage(int width, int height)
{
	this ->width = width;
	this ->height = height;
	
	pixels = NULL;
}

BaseImage::~BaseImage()
{
	Deinitialize();
}

bool BaseImage::Initialize()
{
	pixels = new Color[width * height];
	if (pixels == NULL)
	{
		Deinitialize();
		return false;
	}
	
	return true;
}

void BaseImage::Deinitialize()
{
	MemUtils::DeleteVector(pixels);
}

int BaseImage::GetWidth() const
{
	return width;
}

int BaseImage::GetHeight() const
{
	return height;
}

Color BaseImage::GetColor(int x, int y) const
{
	return pixels[(y * width) + x];
}

const Color *BaseImage::GetColors() const
{
	return pixels;
}