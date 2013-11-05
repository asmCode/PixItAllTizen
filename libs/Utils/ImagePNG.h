#pragma once

#include <windows.h>

#include "Image.h"

class ImagePNG : public Image
{
public: 
	ImagePNG(void);

	void LoadImage(const char *filename);
};
