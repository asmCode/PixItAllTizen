#pragma once

#include "IAudioPlayer.h"
#include <string>
#include <AVFoundation/AVFoundation.h>

class AudioPlayer : public IAudioPlayer
{
private:
	AVAudioPlayer *player;
	
public:
	static AudioPlayer* LoadFromFile(const std::string &file);
	
	void Play();
	void Stop();
	
	void SetLoop(bool loop);
	
	void SetVolume(float vol);
	float GetVolume() const;
};
