#pragma once

#include <string>

class ImageMask
{
private:
	unsigned short width;
	unsigned short height;
	
	unsigned char *data;
	
public:
	ImageMask();
	~ImageMask();
	
	bool Load(const std::string &path);
	unsigned short GetWidth() const;
	unsigned short GetHeight() const;
	
	bool IsVisible(unsigned x, unsigned Y) const;
};
