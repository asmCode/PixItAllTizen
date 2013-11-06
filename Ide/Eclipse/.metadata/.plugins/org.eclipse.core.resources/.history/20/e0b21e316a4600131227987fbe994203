#include "XMLElement.h"

XMLElement::XMLElement()
{
}

XMLElement::XMLElement(const std::string &name)
{
	m_name = name;
}

XMLElement::~XMLElement()
{
	if (m_subElements.size() > 0)
	{
		for (unsigned i = 0; i < m_subElements.size(); i++)
			delete m_subElements[i];
	}
}

bool XMLElement::HasAttribute(const std::string &attribName)
{
	return m_attributes.find(attribName) != m_attributes.end();
}

void XMLElement::AddAttribute(const std::string &name, const std::string &value)
{
	m_attributes[name] = value;
}

const std::string& XMLElement::GetAttribute(const std::string &attribName)
{
	return m_attributes[attribName];
}

bool XMLElement::GetAttributeBool(const std::string &attribName)
{
	int val;
	sscanf(m_attributes[attribName].c_str(), "%d", &val);
	return val != 0;
}

float XMLElement::GetAttributeFloat(const std::string &attribName)
{
	float val;
	sscanf(m_attributes[attribName].c_str(), "%f", &val);
	return val;
}

int XMLElement::GetAttributeInt(const std::string &attribName)
{
	int val;
	sscanf(m_attributes[attribName].c_str(), "%d", &val);
	return val;
}

unsigned XMLElement::GetSubElementsCount() const
{
	return m_subElements.size();
}

void XMLElement::AddSubElement(XMLElement *element)
{
	m_subElements.push_back(element);
}

XMLElement *XMLElement::GetSubElement(unsigned index)
{
	return m_subElements[index];
}

void XMLElement::SetName(const std::string &name)
{
	m_name = name;
}

void XMLElement::SetText(const std::string &text)
{
	m_text = text;
}

const std::string& XMLElement::GetName() const
{
	return m_name;
}

const std::string& XMLElement::GetText() const
{
	return m_text;
}

