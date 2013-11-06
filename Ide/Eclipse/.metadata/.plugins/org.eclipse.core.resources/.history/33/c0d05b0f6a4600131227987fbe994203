#include <fstream>
#include "ImageMask.h"

ImageMask::ImageMask()
{
	width = 0;
	height = 0;
	data = NULL;
}

ImageMask::~ImageMask()
{
	if (data != NULL)
	{
		delete [] data;
		data = NULL;
	}
}

bool ImageMask::Load(const std::string &path)
{
	std::ifstream file(path.c_str(), std::ios::binary);
	if (!file.is_open() || file.fail())
		return false;
	
	file.read((char*)&width, 2);
	file.read((char*)&height, 2);
	
	unsigned extraByte = width * height % 8 > 0 ? 1 : 0;
	unsigned dataSize = (width * height) / 8 + extraByte;
	data = new unsigned char[dataSize];
	file.read((char*)data, dataSize);
	
	file.close();
	
	return true;
}

unsigned short ImageMask::GetWidth() const
{
	return width;
}

unsigned short ImageMask::GetHeight() const
{
	return height;
}

bool ImageMask::IsVisible(unsigned x, unsigned y) const
{
	//return true;
	int bytesShift = y * width + x;
	return ((data[bytesShift / 8] << (bytesShift % 8)) & 0x80) == 0x80;
}
