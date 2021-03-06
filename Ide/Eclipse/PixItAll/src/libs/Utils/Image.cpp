#include "Image.h"
#include <FMedia.h>
#include <FBase.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>

using namespace Tizen::Base;
using namespace Tizen::Graphics;

Image::Image()
{
	width = 0;
	height = 0;
	bpp = 0;
	data = NULL;
}

Image::~Image()
{
	if (data != NULL)
		delete [] (unsigned char*)data;
}

Image* Image::LoadImage(const char *filename)
{
	if (filename == NULL || filename[0] == 0)
	{
		assert(false);
		return NULL;
	}
	
	
	///////////////
	Tizen::Media::Image image;
	image.Construct();

	Bitmap *bitmap = image.DecodeN(filename, BITMAP_PIXEL_FORMAT_R8G8B8A8);
	if (bitmap == null)
	{
		AppLogException("Couldn't load image");
		return NULL;
	}

//	int imgWidth = bitmap->GetWidth();
//	int imgHeight = bitmap->GetHeight();
//	int imgBytesCount = 4;

	int w, h;

	ByteBuffer *buffer = image.DecodeToBufferN(String(filename), BITMAP_PIXEL_FORMAT_R8G8B8A8, w, h);

	int imgWidth = w;
	int imgHeight = h;
	int imgBytesCount = 4;

	unsigned char *data = new unsigned char[imgWidth * imgHeight * imgBytesCount];

	memcpy(data, buffer->GetPointer(), imgWidth * imgHeight * imgBytesCount);

	delete buffer;
	delete bitmap;
	
	
	Image *img = new Image();
	if (img == NULL)
	{
		delete [] data;
		return NULL;
	}
	img ->width = imgWidth;
	img ->height = imgHeight;
	img ->bpp = imgBytesCount * 8;
	img ->data = data;
	
	return img;
}

int Image::GetWidth() const
{
	return width;
}

int Image::GetHeight() const
{
	return height;
}

int Image::GetBpp() const
{
	return bpp;
}

const void *Image::GetData() const
{
	return data;
}
