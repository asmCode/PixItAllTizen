#pragma once

#include <string>

class IContentObserver
{
public:
	virtual void ErrorOccured(const std::string &errorMsg) {};
	virtual void LoadingStarted(int stepsCount) {};
	virtual void ProgressStep() {};
	virtual void LoadingFinished() {};
};
