#pragma once

#include "IImage.h"

class BaseImage : public IImage
{
protected:
	int width;
	int height;
	Color *pixels;
	
	BaseImage(int width, int height);
	bool Initialize();
	
public:
	~BaseImage();
	
	void Deinitialize();
	
	int GetWidth() const;
	int GetHeight() const;
	Color GetColor(int x, int y) const;
	const Color *GetColors() const;
};
