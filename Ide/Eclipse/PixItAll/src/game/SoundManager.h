#pragma once

#include <string>
#include "IAudioPlayer.h"
#include "ObserverSubject.h"
#include "IPropertyObserver.h"
#include "IAsyncable.h"
#include <Utils/GenericSingleton.h>

class SoundManager : public IAsyncable,  public ObserversContainer<IPropertyObserver*>, public virtual GenericSingleton<SoundManager>
{
public:
	enum Sound
	{
		Sound_Button = 0,
		Sound_UnrollPalette,
		Sound_RollPalette,
		Sound_Fail,
		Sound_Victory,
		Sound_Coin,
		
		SoundsCount // always last
	};
	
	static const std::string SoundFiles[];
	
private:
	static const unsigned PixelSoundCount = 20; //TEST
	static const unsigned BonusSoundCount = 3; //TEST
	
	float soundVolume;
	float musicVolume;
	
	IAudioPlayer *music;
	
	IAudioPlayer *pixelSounds[PixelSoundCount];
	IAudioPlayer *bonusSounds[BonusSoundCount];
	
	IAudioPlayer *sounds[SoundsCount];
	
	std::string basePath;
	
	bool isLoaded;
		
public:
	SoundManager();
	~SoundManager();

	void SetAudioBasePath(const std::string &audioPath);
	void RunAsync();
	
	bool Initialize(const std::string &audioPath);
	
	void SetSoundVolume(float value);
	void SetMusicVolume(float value);

	float GetSoundVolume() const;
	float GetMusicVolume() const;
	
	void PlayMusic();
	void StopMusic();
	
	void PlayPixel();
	void PlayBonus(unsigned bonusLevel);
	
	void PlaySound(Sound sound);
	
	bool IsLoaded() const;
};
