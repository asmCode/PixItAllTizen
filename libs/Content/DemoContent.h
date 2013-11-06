#pragma once

#include <string>
#include <map>

#import "../Utils/IServiceProvider.h"
#import "../Utils/GenericSingleton.h"

#include "TextureLoader.h"
#include "AssetsManager.h"
#include <typeinfo>

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
					   const char *texSubPath);
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
