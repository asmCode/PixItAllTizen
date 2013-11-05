#ifndef SSG_XMLDOCUMENT_H
#define SSG_XMLDOCUMENT_H

#include <string>
#include <map>

class XMLElement;
class TiXmlElement;

class XMLDocument
{
private:
	XMLElement *m_rootElement;
	
	static XMLElement *LoadElement(const TiXmlElement *tiElement);
	
public:
	XMLDocument();
	~XMLDocument();
	
	static XMLDocument *LoadFromXmlFile(const std::string &xmlFile);
	
	XMLElement *GetRootElement();
};

#endif
