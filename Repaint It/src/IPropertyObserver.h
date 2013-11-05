#pragma once

#include <string>

class IPropertyObserver
{
public:
	virtual void PropertyChanged(const std::string &propName,
								 void *sender) = 0;
};
