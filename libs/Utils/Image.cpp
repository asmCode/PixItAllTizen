#include "Image.h"
#include <stddef.h>
#import <UIKit/UIImage.h>

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
	
	NSString *strFilename = [NSString stringWithUTF8String:filename];
	if (strFilename == NULL)
		return NULL;
	
	UIImage *uiImage = [[UIImage alloc] initWithContentsOfFile:strFilename];
	if (uiImage == NULL)
		return NULL;
	
	CGImage *cgImage = uiImage.CGImage;
	
	CGDataProvider *dataProvider = CGImageGetDataProvider(cgImage);
	if (dataProvider == NULL)
		return NULL;
	
	CFDataRef data = CGDataProviderCopyData(dataProvider);
	if (data == NULL)
		return NULL;
	
	int imgWidth = CGImageGetWidth(cgImage);
	int imgHeight = CGImageGetHeight(cgImage);
	int imgBpp = CGImageGetBitsPerPixel(cgImage);
	unsigned char *imgData = new unsigned char[CGImageGetBytesPerRow(cgImage) * imgHeight];
	if (imgData == NULL)
		return NULL;
	
	CFDataGetBytes(data, CFRangeMake(0, CFDataGetLength(data)), imgData);

	[uiImage release];
	CFRelease(data);
	//CFRelease(dataProvider);
	
	Image *img = new Image();
	if (img == NULL)
	{
		delete [] imgData;
		return NULL;
	}
	img ->width = imgWidth;
	img ->height = imgHeight;
	img ->bpp = imgBpp;
	img ->data = imgData;
	
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
