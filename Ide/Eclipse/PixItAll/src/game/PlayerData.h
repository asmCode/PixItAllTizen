#pragma once

#include <string>

class PlayerData
{
public:
	static PlayerData* GetInstance();

	void Initialize(const std::string &path);

	void Load();
	void Save();

	std::string m_name;
	int m_id;

private:
	static PlayerData* m_instance;
	std::string m_path;

	PlayerData();
};

