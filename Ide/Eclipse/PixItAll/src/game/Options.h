#pragma once

#include <Utils/GenericSingleton.h>
#include <string>

class Options : public GenericSingleton<Options>
{
private:
	std::string path;
	
	float musicVol;
	float soundsVol;
	bool showFinishedAllLevels;
	bool showAskForRate;
	int askForRateAfterFinishedLevels;
	
public:
	Options();
	
	void SetOptionsFile(const std::string &path);
	bool Load();
	void Save();
	
	float GetMusicVolume() const;
	void SetMusicVolume(float volume);
	
	float GetSoundsVolume() const;
	void SetSoundsVolume(float volume);
	
	bool GetShowFinishedAllLevels();
	void SetShowFinishedAllLevels(bool val);
	
	bool GetAskForRate();
	void SetAskForRate(bool val);
	
	int GetAskForRateAfterFinishedLevels() const;
};
