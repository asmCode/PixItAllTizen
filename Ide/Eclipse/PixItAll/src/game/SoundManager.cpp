#include "SoundManager.h"
#include <stdio.h>
#include <assert.h>
#include <Utils/Log.h>
#include <stdlib.h>
#include <memory.h>

#include "AudioPlayerFactory.h"

const std::string SoundManager::SoundFiles[] =
{
	std::string("button.mp3"),
	std::string("palette_unroll.mp3"),
	std::string("palette_roll.mp3"),
	std::string("fail.mp3"),
	std::string("victory.mp3"),
	std::string("coin.mp3"),
};

SoundManager::SoundManager()
{
	soundVolume = 0.5f;
	musicVolume = 0.5f;
	
	music = NULL;
	isLoaded = false;
	
	memset(pixelSounds, 0, sizeof(IAudioPlayer*) * PixelSoundCount);
	memset(bonusSounds, 0, sizeof(IAudioPlayer*) * BonusSoundCount);
	memset(sounds, 0, sizeof(IAudioPlayer*) * SoundsCount);
}

SoundManager::~SoundManager()
{
}

void SoundManager::SetAudioBasePath(const std::string &audioPath)
{
	this->basePath = audioPath;
}

void SoundManager::RunAsync()
{
	assert(!basePath.empty());
	
	isLoaded = false;
	Initialize(basePath);
	isLoaded = true;
	
	ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, "loadingFinished", this);
}

bool SoundManager::Initialize(const std::string &audioPath)
{
	music = AudioPlayerFactory::CreateTizenAudioPlayer(audioPath + "music01.mp3");
	music->SetLoop(true);
	music->SetVolume(musicVolume);
	
	char soundFile[32];
	
	for (unsigned i = 0; i < PixelSoundCount; i++)
	{
		sprintf(soundFile, "pixel_%.2d.mp3", i);
		Log::LogT("loading: %s", soundFile);
		pixelSounds[i] = AudioPlayerFactory::CreateAlAudioPlayer(audioPath + soundFile, true);
		pixelSounds[i]->SetVolume(soundVolume);
	}
	
	for (unsigned i = 0; i < BonusSoundCount; i++)
	{
		sprintf(soundFile, "pixel_s_%.2d.mp3", i);
		Log::LogT("loading: %s", soundFile);
		bonusSounds[i] = AudioPlayerFactory::CreateAlAudioPlayer(audioPath + soundFile, true);
		bonusSounds[i]->SetVolume(soundVolume);
	}
	
	for (unsigned i = 0; i < SoundsCount; i++)
	{
		Log::LogT("loading: %s", SoundFiles[i].c_str());
		sounds[i] = AudioPlayerFactory::CreateAlAudioPlayer(audioPath + SoundFiles[i], false);
		sounds[i]->SetVolume(soundVolume);
	}
	
	return true;
}

void SoundManager::SetSoundVolume(float value)
{
	this->soundVolume = value;
	
	for (unsigned i = 0; i < PixelSoundCount; i++)
		pixelSounds[i]->SetVolume(value);
	
	for (unsigned i = 0; i < BonusSoundCount; i++)
		bonusSounds[i]->SetVolume(value);
	
	for (unsigned i = 0; i < SoundsCount; i++)
		sounds[i]->SetVolume(value);
}

void SoundManager::SetMusicVolume(float value)
{
	this->musicVolume = value;
	
	music->SetVolume(value);
}

float SoundManager::GetSoundVolume() const
{
	return soundVolume;
}

float SoundManager::GetMusicVolume() const
{
	return musicVolume;
}

void SoundManager::PlayMusic()
{
	//return;
	
	assert(music != NULL);
	music->Play();
}

void SoundManager::StopMusic()
{
	music->Stop();
}

void SoundManager::PlayPixel()
{
	pixelSounds[rand() % PixelSoundCount]->Play();
}

void SoundManager::PlayBonus(unsigned bonusLevel)
{
	bonusSounds[bonusLevel]->Play();
}

bool SoundManager::IsLoaded() const
{
	return isLoaded;
}

void SoundManager::PlaySound(SoundManager::Sound sound)
{
	sounds[sound]->Play();
}
