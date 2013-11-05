#pragma once

#include <string>
#include "IAudioPlayer.h"

class AudioPlayerFactory
{
public:
	static IAudioPlayer* CreateAVAudioPlayer(const std::string &file);
	static IAudioPlayer* CreateAlAudioPlayer(const std::string &file, bool stereo);
	static IAudioPlayer* CreateAqAudioPlayer(const std::string &file);
};
