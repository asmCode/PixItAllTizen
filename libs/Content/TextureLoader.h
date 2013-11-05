#pragma once

#import "../Utils/IServiceProvider.h"
#include "IAssetLoader.h"

class TextureLoader : public IAssetLoader
{
public:
	void *Load(const char *path);
};
