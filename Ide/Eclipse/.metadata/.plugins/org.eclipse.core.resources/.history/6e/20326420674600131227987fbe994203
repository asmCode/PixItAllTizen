#pragma once

#include <string>
#include <map>

#import "../Utils/IServiceProvider.h"
#import "../Utils/GenericSingleton.h"

#include "ModelLoader.h"
#include "TextureLoader.h"
#include "AnimationLoader.h"
#include "PVRTextureLoader.h"
#include "AssetsManager.h"

class IContentObserver;

class DemoContent : public GenericSingleton<DemoContent>
{
	friend class GenericSingleton<DemoContent>;

private:
	std::map<std::string, AssetsManager*> assetsManagers;
	sm::IServiceProvider *serviceProvider;

	DemoContent();
	~DemoContent();

	std::vector<IContentObserver*> contObs;

public:
	void AddContentObserver(IContentObserver *contObs);
	
	bool LoadResources(const char *basePath,
					   const char *texSubPath,
					   const char *modelSubPath,
					   const char *animSubPath,
					   sm::IServiceProvider *serviceProvider);
	void ReleaseResources();

	template <typename T>
	T Get(const char *name)
	{
		std::map<std::string, AssetsManager*>::iterator it = assetsManagers.find(typeid(T).name());
		if (it == assetsManagers.end())
			return NULL;

		return it ->second ->Get<T>(name);
	}

	template <typename T>
	void GetAll(std::vector<T> &assets)
	{
		std::map<std::string, AssetsManager*>::iterator it = assetsManagers.find(typeid(T).name());
		if (it == assetsManagers.end())
			return;

		it ->second ->GetAll<T>(assets);
	}
};
