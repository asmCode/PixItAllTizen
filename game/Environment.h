#ifndef SSG_ENVIRONMENT_H
#define SSG_ENVIRONMENT_H

#include <Utils/GenericSingleton.h>
#include <string>

class Environment : public virtual GenericSingleton<Environment>
{
private:
	unsigned m_screenWidth;
	unsigned m_screenHeight;
	std::string m_iosVersion;
	
public:
	Environment();
	
	void SetScreenSize(unsigned width, unsigned height);
	void SetIOSVersion(const std::string &version);
	
	unsigned GetScreenWidth() const;
	unsigned GetScreenHeight() const;
	std::string GetIOSVersion() const;
};

#endif
