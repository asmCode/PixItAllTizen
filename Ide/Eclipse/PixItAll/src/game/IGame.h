#pragma once

#include "IGestureHandler.h"
#include <string>

class ISystemUtils;

class IGame : public IGestureHandler
{
public:
	virtual ~IGame() {};
	virtual bool Initialize(const std::string &basePath,
							const std::string &docPath,
							ISystemUtils *systemUtils) = 0;
	virtual void Deinitialize() = 0;
	virtual void Update(float time, float ms) = 0;
	virtual void Draw(float time, float ms) = 0;
	
	virtual void HandleEnterBackground() = 0;
	virtual void HandleEnterForeground() = 0;
	virtual void HandleBackButton() = 0;
	virtual void HandleMenukButton() = 0;
};
