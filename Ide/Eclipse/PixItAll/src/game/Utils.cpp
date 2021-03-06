#include "Utils.h"
#define TIXML_USE_STL
#include <Utils/Xml/tinyxml.h>
#include <Content/DemoContent.h>
#include <Utils/Utils/Path.h>
#include <GraphicsLibrary/TexPart.h>
#include <FIo.h>

#include "XMLDocument.h"
#include <FBase.h>

using namespace Tizen::Io;
using namespace Tizen::Base;

bool Utils::ParseBounds(const std::string &strBounds,
					   sm::Rect<int> &bounds)
{
	int x;
	int y;
	int width;
	int height;
	
	if (sscanf(strBounds.c_str(), "%d,%d,%d,%d", &x, &y, &width, &height) != 4)
		return false;
	
	bounds = sm::Rect<int>(x, y, width, height);
	return true;
}

bool Utils::LoadGuiMap(const std::string &path)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	DemoContent *dc = DemoContent::GetInstance();

	TiXmlDocument xmlDoc;
	if (!xmlDoc.LoadFile(path))
		return false;
	
	const TiXmlElement *rootNode = xmlDoc.RootElement();
	if (rootNode == NULL || (strcmp(rootNode ->Value(), "texmap") != 0))
		return false;
			
	const TiXmlNode *child = NULL;
	while((child = rootNode->IterateChildren(child)) != NULL)
	{
		const TiXmlElement *charElement = child ->ToElement();
		if (strcmp(charElement ->Value(), "area") == 0)
		{
			std::string file;
			std::string name;
			std::string bounds;
			if (charElement ->QueryStringAttribute("file", &file) != TIXML_SUCCESS ||
				charElement ->QueryStringAttribute("name", &name) != TIXML_SUCCESS ||
				charElement ->QueryStringAttribute("bounds", &bounds) != TIXML_SUCCESS)
				continue;
			
			sm::Rect<int> boundsValues;
			if (!ParseBounds(bounds, boundsValues))
				continue;
			
			Path filePath(file);
			Texture *tex = dc->Get<Texture*>(filePath.GetFilename().c_str());
			assert(tex != NULL && "resource not found");
			
			cc ->AddClass("guimap_" + name, TexPart(tex, boundsValues));
		}
	}
	
	return true;
}

std::string Utils::CalcMD5(const std::string &str)
{
	return str;
}

bool Utils::CreateDir(const std::string &dir)
{
	return Directory::Create(String(dir.c_str()), true) == E_SUCCESS;
}

void Utils::LoadGuiProperties(const std::string &path)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	
	std::vector<std::string> filesNames;
	Path::GetAllFiles(filesNames, path, "*.xml");
	
	for (unsigned i = 0; i < filesNames.size(); i++)
	{
		XMLDocument *xmlDoc = XMLDocument::LoadFromXmlFile(path + filesNames[i]);
		assert(xmlDoc != NULL);
		
		Path filenamePath(filesNames[i]);
		
		cc->AddClass("guidef_" + filenamePath.GetFilename(), xmlDoc->GetRootElement());
	}
}
