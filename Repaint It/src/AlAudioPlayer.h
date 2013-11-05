#pragma once

#include "IAudioPlayer.h"
#include <assert.h>
#include <string>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAl/oalStaticBufferExtension.h>

#include <OpenAl/oalStaticBufferExtension.h>

extern alBufferDataStaticProcPtr alBufferDataStatic;

class AlAudioPlayer : public IAudioPlayer
{
private:
	static ALCdevice *audioDev;
	static ALCcontext *audioContext;
	static bool isInitialized;
	
	ALuint sourceName;
	ALuint bufferName;
	char *audioData;
	
public:
	AlAudioPlayer();
	~AlAudioPlayer();
	
	static bool IsInitialized();
	static bool Initialize();
	static void Deinitialize();
	
	static AlAudioPlayer* LoadFromFile(const std::string &file, bool stereo);
	
	void Play();
	void Stop();	
	void SetLoop(bool loop);
	void SetVolume(float vol);
	float GetVolume() const;
};
