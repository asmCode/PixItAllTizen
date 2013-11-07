#pragma once

#include <map>
#include <string>
#include "IServiceProvider.h"

class ServiceProvider : public sm::IServiceProvider
{
private:
	std::map<std::string, void*> services;

public:
	void AddService(const char *serviceType, void *service)
	{
		services[serviceType] = service;
	}

	void *GetService(const char *typeName)
	{
		std::map<std::string, void*>::iterator it = services.find(typeName);
		if (it == services.end())
			return NULL;

		return it ->second;
	}
};
