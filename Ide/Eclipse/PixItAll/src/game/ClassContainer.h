#pragma once

#include <map>
#include <assert.h>
#include <string>
#include <Utils/GenericSingleton.h>
#include <memory.h>

class ClassContainer : public GenericSingleton<ClassContainer>
{
private:
	std::map<std::string, unsigned char*> classes;
	
	~ClassContainer();
	
public:
	template <typename T>
	void AddClass(const std::string &name, const T &value)
	{
		unsigned char *valCopy = new unsigned char[sizeof(T)];
		memcpy(valCopy, &value, sizeof(T));
		classes[name] = valCopy;
	}
	
	template <typename T>
	bool TryGetClass(const std::string &name, T &value)
	{
		std::map<std::string, unsigned char*>::iterator it = classes.find(name);
		if (it == classes.end())
			return false;
		
		memcpy(&value, it ->second, sizeof(T));
		return true;
	}
	
	template <typename T>
	T GetClass(const std::string &name)
	{
		T ret;
		if (!TryGetClass(name, ret))
		{
			assert(false && "class not found");
		}
		
		return ret;
	}
};
