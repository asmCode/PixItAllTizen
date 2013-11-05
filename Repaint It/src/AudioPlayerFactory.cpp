#include "AudioPlayerFactory.h"
#include "AudioPlayer.h"
#include "AlAudioPlayer.h"
#include "AqAudioPlayer.h"
#include "Log.h"

IAudioPlayer* AudioPlayerFactory::CreateAVAudioPlayer(const std::string &file)
{
	return AudioPlayer::LoadFromFile(file);
}

IAudioPlayer* AudioPlayerFactory::CreateAlAudioPlayer(const std::string &file, bool stereo)
{
	if (!AlAudioPlayer::IsInitialized())
	{
		if (!AlAudioPlayer::Initialize())
		{
			assert(false);
			Log::LogE("Couldn't initialize AlAudioPlayer");
			return NULL;
		}
	}
	
	return AlAudioPlayer::LoadFromFile(file, stereo);
}

IAudioPlayer* AudioPlayerFactory::CreateAqAudioPlayer(const std::string &file)
{
	return AqAudioPlayer::LoadFromFile(file);
}
