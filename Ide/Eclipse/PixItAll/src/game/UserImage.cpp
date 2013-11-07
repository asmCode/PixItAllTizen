#include "UserImage.h"
#include <stddef.h>
#include <memory.h>

UserImage::UserImage(int width, int height) : BaseImage(width, height)
{
}

UserImage* UserImage::Create(int width, int height)
{
	UserImage *ret = new UserImage(width, height);
	if (ret == NULL)
		return NULL;
	
	if (!ret ->Initialize())
	{
		ret ->Deinitialize();
		return ret;
	}
	
	ret ->resolvedColors = new bool[width * height];
	if (ret ->resolvedColors == NULL)
		return NULL;
	
	ret ->resolvedCount = 0;
	
	return ret;
}

void UserImage::SetColor(int x, int y, const Color &color)
{
	if (!resolvedColors[y * width + x])
	{
		resolvedCount++;
		resolvedColors[y * width + x] = true;
	}
	
	pixels[y * width + x] = color;
}

bool UserImage::IsColorResolved(int x, int y) const
{
	return resolvedColors[y * width + x];
}

void UserImage::Clear(const Color &color)
{
	for (int i = 0; i < width * height; i++)
		pixels[i] = color;
	
	memset(resolvedColors, 0, sizeof(bool) * width * height);
}

unsigned UserImage::GetTotalPixels() const
{
	return width * height;
}

unsigned UserImage::GetResolvedPixels() const
{
	return resolvedCount;
}
