#include "DemoContent.h"
#include "../GraphicsLibrary/Texture.h"
#include <typeinfo>

template<typename T> extern T* GenericSingleton<T>::instance;

DemoContent::DemoContent()
{
}

DemoContent::~DemoContent()
{
}

void DemoContent::AddContentObserver(IContentObserver *contObs)
{
	this ->contObs.push_back(contObs);
}
	
bool DemoContent::LoadResources(const char *basePath,
								const char *texSubPath)
{
	std::string basePathStr(basePath);
	std::string texPath = basePathStr + texSubPath;

	std::vector<std::string> pvrFiles;

	Path::GetAllFiles(pvrFiles, texPath, "png");

	bool result = true;

	this ->serviceProvider = serviceProvider;
	
	assetsManagers[typeid(Texture*).name()] =
		new AssetsManager(new TextureLoader(), texPath.c_str(), "png");
	
	//kolejnosc jest istotna... (juz nie, bo nie laczymy resourcow podczas ladowania!)
	//result &= assetsManagers[typeid(Texture*).name()] ->Load(true);
	result &= assetsManagers[typeid(Texture*).name()] ->Load(true);

	return result;
}

void DemoContent::ReleaseResources()
{
	std::map<std::string, AssetsManager*>::iterator it;
	for (it = assetsManagers.begin(); it != assetsManagers.end(); it++)
		delete it ->second;

	assetsManagers.clear();
}
