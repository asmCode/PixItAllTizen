#pragma once

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "../Utils/IServiceProvider.h"
#include "IAssetLoader.h"

class PVRTextureLoader : public IAssetLoader
{
private:
	char *GetFileData(const char *path);
	
public:
	void *Load(const char *path, sm::IServiceProvider *serviceProvider);
};