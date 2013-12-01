#include "PlayerData.h"
#include <Utils/StringUtils.h>
#include <Utils/Utils/Path.h>
#include <Utils/Log.h>
#include <Utils/Xml/XMLLoader.h>
#include <Utils/Xml/XMLNode.h>
#include <string>

PlayerData* PlayerData::m_instance;

PlayerData* PlayerData::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new PlayerData();

	return m_instance;
}

PlayerData::PlayerData() :
	m_name("unnamed"),
	m_id(0)
{
}

void PlayerData::Initialize(const std::string &path)
{
	m_path = path;
}

void PlayerData::Load()
{
	if (!Path::IsFileExists(m_path.c_str()))
		return;

	XMLNode *node = XMLLoader::LoadFromFile(m_path);
	if (node == NULL)
		return;

	for (int i = 0; i < node->GetChildrenCount(); i++)
	{
		XMLNode *child = node->GetChild(i);
		if (child->GetName() == "Name")
			m_name = child->GetValueAsString();
		else if (child->GetName() == "Id")
			m_id = child->GetValueAsInt32();
	}

	Log::LogT("Player loaded (%s, %d)", m_name.c_str(), m_id);
}

void PlayerData::Save()
{
	std::string xml;

	xml += "<Player>\n";
	xml += "\t<Name>"; xml += m_name; xml += "</Name>\n";
	xml += "\t<Id>"; xml += StringUtils::ToString(m_id); xml += "</Id>\n";
	xml += "</Player>\n";

	Path::WriteTextFile(m_path.c_str(), xml);
}
