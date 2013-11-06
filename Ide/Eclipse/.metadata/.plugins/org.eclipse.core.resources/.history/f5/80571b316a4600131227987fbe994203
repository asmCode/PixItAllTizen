#define TIXML_USE_STL
#include <Utils/Xml/tinyxml.h>

#include "XMLDocument.h"
#include "XMLElement.h"

XMLDocument::XMLDocument()
{
	m_rootElement = NULL;
}

XMLDocument::~XMLDocument()
{
	if (m_rootElement != NULL)
		delete m_rootElement;
}

XMLElement *XMLDocument::LoadElement(const TiXmlElement *tiElement)
{
	XMLElement *xmlElement = new XMLElement();
	
	const char *value = tiElement->Value();
	if (value != NULL)
		xmlElement->SetName(value);
	
	const char *text = tiElement->GetText();
	if (text != NULL)
		xmlElement->SetText(text);
	
	const TiXmlAttribute *tiAttrib = tiElement->FirstAttribute();
	while (tiAttrib != NULL)
	{
		xmlElement->AddAttribute(tiAttrib->Name(), tiAttrib->Value());
		tiAttrib = tiAttrib->Next();
	}
	
	const TiXmlElement *childElement = tiElement->FirstChildElement();
	
	while (childElement != NULL)
	{
		xmlElement->AddSubElement(LoadElement(childElement));
				
		childElement = childElement->NextSiblingElement();
	}
	
//	const TiXmlElement *siblingElement = tiElement->NextSiblingElement();
//	
//	while (childElement != NULL)
//	{
//		xmlElement->AddSubElement(LoadElement(childElement));
//		
//		childElement = childElement->NextSiblingElement();
//	}

	return xmlElement;
}

XMLDocument *XMLDocument::LoadFromXmlFile(const std::string &xmlFile)
{
	TiXmlDocument tiDoc;
	if (!tiDoc.LoadFile(xmlFile))
		return NULL;
	
	const TiXmlElement *tiRootNode = tiDoc.RootElement();
	if (tiRootNode == NULL)
		return NULL;
	
	XMLElement *rootElement = LoadElement(tiRootNode);
	
	if (rootElement == NULL)
		return NULL;
	
	XMLDocument *doc = new XMLDocument();
	doc->m_rootElement = rootElement;
	
	return doc;
}

XMLElement *XMLDocument::GetRootElement()
{
	return m_rootElement;
}
