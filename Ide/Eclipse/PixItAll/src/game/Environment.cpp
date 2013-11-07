#include "Environment.h"

namespace PixItAll
{

Environment::Environment()
{
	m_screenWidth = 0;
	m_screenHeight = 0;
}

void Environment::SetScreenSize(unsigned width, unsigned height)
{
	this->m_screenWidth = width;
	this->m_screenHeight = height;
}

void Environment::SetIOSVersion(const std::string &iosVersion)
{
	this->m_iosVersion = iosVersion;
}

unsigned Environment::GetScreenWidth() const
{
	return m_screenWidth;
}

unsigned Environment::GetScreenHeight() const
{
	return m_screenHeight;
}

std::string Environment::GetIOSVersion() const
{
	return m_iosVersion;
}

} // namespace PixItAll

