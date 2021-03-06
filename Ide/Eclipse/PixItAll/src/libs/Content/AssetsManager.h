#pragma once

#import "../Utils/Utils/Path.h"

#include "IAssetLoader.h"

#import "../Utils/sb.h"
#import "../Utils/IDisposable.h"
#include "IAssetsManager.h"
#include <string>
#include <map>

class AssetsManager : public IAssetsManager
{
protected:
	std::map<std::string, IDisposable*> items;
	IAssetLoader *assetLoader;
	std::string basePath;
	std::vector<std::string> assetsExtensions;

public:
	AssetsManager(IAssetLoader *assetLoader,
		const char *basePath, const char *assetExtensions)
	{
		this ->assetLoader = assetLoader;
		this ->basePath = basePath;
		this ->assetsExtensions.push_back(assetExtensions);
		
		Path::FixFoldername(this ->basePath);
	}

	~AssetsManager()
	{
		delete assetLoader;

		std::map<std::string, IDisposable*>::iterator it;
		for (it = items.begin(); it != items.end(); it++)
			delete it ->second;

		items.clear();
	}

	void Add(const char *name, void *asset)
	{
		items[name] = (IDisposable*)asset;
	}

	template <typename T>
	T Get(const char *name)
	{
		std::map<std::string, IDisposable*>::iterator i = items.find(name);
		if (i == items.end())
			return NULL;

		return (T)(*i).second;
	}

	template <typename T>
	void Get(int i, char *name, T &asset)
	{
		typename std::map<std::string, T>::iterator it = items.begin();
		while (i--)
			it ++;

		if (name != NULL)
			strcpy(name, (*it).first.c_str());
		asset = (*it).second;
	}

	template <typename T>
	void GetAll(std::vector<T> &assets)
	{
		//std::copy(items.begin(), items.end(), assets);

		std::map<std::string, IDisposable*>::iterator it;
		for (it = items.begin(); it != items.end(); it++)
			assets.push_back((T)it ->second);
	}

	virtual int GetCount()
	{
		return (int)items.size();
	}

	virtual bool Load(bool reportProgress)
	{
		bool result = true;

		std::string searchFilter;
		searchFilter += "*.";
		searchFilter += assetsExtensions[0];
		
		std::vector<std::string> assetFileNames;
		Path::GetAllFiles(assetFileNames, basePath, searchFilter);
		
		for (unsigned i = 0; i < assetFileNames.size(); i++)
		{
			Path assetPath(basePath + assetFileNames[i]);
			void *asset = assetLoader ->Load(assetPath.GetFullPath().c_str());
			if (asset == NULL)
			{
				result = false; // remember error, but continue logging next errors, if any
			}
			else
				Add(assetPath.GetFilename().c_str(), asset);
		}
		
		return result;
	}
};
