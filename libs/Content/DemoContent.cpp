#include "DemoContent.h"

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
								const char *texSubPath,
								const char *modelSubPath,
								const char *animSubPath,
								sm::IServiceProvider *serviceProvider)
{
	std::string basePathStr(basePath);
	std::string texPath = basePathStr + texSubPath;
	std::string modelPath = basePathStr + modelPath;
	std::string animPath = basePathStr + modelPath;

	std::vector<std::string> pvrFiles;
	std::vector<std::string> mdlFiles;
	std::vector<std::string> aniFiles;

	Path::GetAllFiles(pvrFiles, texPath, "*.png");
	Path::GetAllFiles(mdlFiles, modelPath, "*.geo");
	Path::GetAllFiles(aniFiles, modelPath, "*.ani");

	bool result = true;

	this ->serviceProvider = serviceProvider;
	
	ModelLoader *modelLoader = new ModelLoader();
	AnimationLoader *animLoader = new AnimationLoader();

	assetsManagers[typeid(Texture*).name()] =
		new AssetsManager(new TextureLoader(), texPath.c_str(), "png");
	
	assetsManagers[typeid(Model*).name()] =
		new AssetsManager(modelLoader, modelPath.c_str(), "geo");
	
	assetsManagers[typeid(Animation*).name()] =
		new AssetsManager(animLoader, animPath.c_str(), "ani");
	
	//kolejnosc jest istotna... (juz nie, bo nie laczymy resourcow podczas ladowania!)
	//result &= assetsManagers[typeid(Texture*).name()] ->Load(true);
	result &= assetsManagers[typeid(Texture*).name()] ->Load(true);
	result &= assetsManagers[typeid(Model*).name()] ->Load(true);
	result &= assetsManagers[typeid(Animation*).name()] ->Load(true);

	return result;
}

void DemoContent::ReleaseResources()
{
	std::map<std::string, AssetsManager*>::iterator it;
	for (it = assetsManagers.begin(); it != assetsManagers.end(); it++)
		delete it ->second;

	assetsManagers.clear();
}
