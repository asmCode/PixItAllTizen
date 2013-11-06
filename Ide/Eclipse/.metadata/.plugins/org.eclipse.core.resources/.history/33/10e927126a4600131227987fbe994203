#include "LevelStats.h"
#include <assert.h>
#include <Utils/Utils/Path.h>
#include <sstream>
#include <fstream>

#define TIXML_USE_STL
#include <Utils/Xml/tinyxml.h>

LevelStats::LevelStats(const std::string &path)
{
	this->path = path;
	
	this->points = 0;
	
	//this->unlocked = true;
	this->unlocked = false; // TEST
}

bool LevelStats::Load()
{
	assert(!path.empty());
	
	if (!Path::IsFileExists(path.c_str()))
		return true;
	
	TiXmlDocument xmlDoc;
	if (!xmlDoc.LoadFile(path))
		return false;
	
	const TiXmlElement *rootNode = xmlDoc.RootElement();
	if (rootNode == NULL || (strcmp(rootNode ->Value(), "image_stats") != 0))
		return false;
	
	const TiXmlElement *rootElement = rootNode->ToElement();
	if (rootElement == NULL)
		return false;
		
	if (rootElement ->QueryValueAttribute("points", &points) != TIXML_SUCCESS ||
		rootElement ->QueryValueAttribute("unlocked", &unlocked) != TIXML_SUCCESS)
		return false;
	
	return true;
}

bool LevelStats::Save()
{
	assert(!path.empty());
	
	std::stringstream ss;
	ss << "<image_stats points=\"" << points << "\" unlocked=\"" << (unlocked ? "1" : "0") << "\"/>";
	
	std::ofstream file(path.c_str(), std::ios::trunc);
	if (file.fail())
		return false;
	
	file.write(ss.str().c_str(), ss.str().length());
	file.close();
	
	return true;
}

unsigned LevelStats::GetPoints() const
{
	return points;
}

bool LevelStats::IsUnlocked() const
{
	return unlocked;
}

void LevelStats::SetPoints(unsigned points)
{
	this->points = points;
}

void LevelStats::SetLock(bool lock)
{
	this->unlocked = !lock;
}
