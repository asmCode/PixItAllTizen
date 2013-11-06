#pragma once

#include "BaseImage.h"

class Image;

class RefImage : public BaseImage
{
private:
	Image *image;
	
	RefImage(int width, int height);
	
public:
	static RefImage* Create(Image *image);
	
	bool CheckColor(int x, int y, const Color &color);
	const Image* GetImage() const;
};
