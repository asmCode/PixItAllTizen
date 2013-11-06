#pragma once

#include <string>

class LevelStats
{
private:
	std::string path;
	
	unsigned points;
	bool unlocked;
	
public:
	LevelStats(const std::string &path);
	
	bool Load();
	bool Save();
	
	unsigned GetPoints() const;
	bool IsUnlocked() const;
	
	void SetPoints(unsigned points);
	void SetLock(bool lock);
};
