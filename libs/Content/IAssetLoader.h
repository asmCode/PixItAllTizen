#ifndef _IASSETLOADER_
#define _IASSETLOADER_

class IAssetLoader
{
public:
	virtual ~IAssetLoader() {}
	virtual void *Load(const char *path) = 0;
};

#endif
