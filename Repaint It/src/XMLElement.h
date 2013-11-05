#ifndef SSG_XMLELEMENT_H
#define SSG_XMLELEMENT_H

#include <string>
#include <map>
#include <vector>

class XMLElement
{
private:
	std::string m_name;
	std::string m_text;
	std::map<std::string, std::string> m_attributes;
	std::vector<XMLElement*> m_subElements;
	
public:
	XMLElement();
	XMLElement(const std::string &name);
	~XMLElement();
	
	bool HasAttribute(const std::string &attribName);
	void AddAttribute(const std::string &name, const std::string &value);
		
	const std::string& GetAttribute(const std::string &attribName);
	bool GetAttributeBool(const std::string &attribName);
	float GetAttributeFloat(const std::string &attribName);
	int GetAttributeInt(const std::string &attribName);

	unsigned GetSubElementsCount() const;
	void AddSubElement(XMLElement *element);
	XMLElement *GetSubElement(unsigned index);
	
	void SetName(const std::string &name);
	void SetText(const std::string &text);
	
	const std::string& GetName() const;
	const std::string& GetText() const;
};

#endif
