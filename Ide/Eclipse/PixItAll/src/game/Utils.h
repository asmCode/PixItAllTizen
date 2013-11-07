#pragma once

#include <string>
#include "ClassContainer.h"
#include <GraphicsLibrary/Rect.h>

class Utils
{
private:
	static bool ParseBounds(const std::string &strBounds,
						   sm::Rect<int> &bounds);
	
public:
	static bool LoadGuiMap(const std::string &path);
	static std::string CalcMD5(const std::string &str);
	static bool CreateDir(const std::string &dir);
	static void LoadGuiProperties(const std::string &path);
};
