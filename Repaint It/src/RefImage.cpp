#include "RefImage.h"
#include <Utils/Image.h>
#include <assert.h>

RefImage::RefImage(int width, int height) : BaseImage(width, height)
{
}

RefImage* RefImage::Create(Image *image)
{
	assert(image != NULL);
	
	RefImage *ret = new RefImage(image ->GetWidth(), image ->GetHeight());
	if (ret == NULL)
		return NULL;
	
	ret->image = image;
	
	if (!ret ->Initialize())
	{
		ret ->Deinitialize();
		return NULL;
	}
	
	int bytespp = image ->GetBpp() / 8;
	const unsigned char *data = (const unsigned char *)image ->GetData();
	int pixelsIndex;
	int dataIndex;
	
	for (int x = 0; x < ret ->width; x++)
	{
		for (int y = 0; y < ret ->height; y++)
		{
			pixelsIndex = y * ret ->width + x;
			dataIndex = pixelsIndex * bytespp;
			ret ->pixels[pixelsIndex] = Color(data[dataIndex + 0], data[dataIndex + 1], data[dataIndex + 2]);
		}
	}
	
	return ret;
}

bool RefImage::CheckColor(int x, int y, const Color &color)
{
	return pixels[y * width + x] == color;
}

const Image* RefImage::GetImage() const
{
	return image;
}
