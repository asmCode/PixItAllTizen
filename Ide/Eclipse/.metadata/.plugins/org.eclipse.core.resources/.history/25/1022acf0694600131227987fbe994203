#include "AudioPlayer.h"
#include "Log.h"
#include <OpenAl/al.h>

AudioPlayer* AudioPlayer::LoadFromFile(const std::string &file)
{
	NSError *err = nil;
	NSURL *soundFileURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:file.c_str()]];
	AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithContentsOfURL:soundFileURL error:&err];
	
	if (err != nil)
	{
		Log::LogE("Couldn't load audio file: %s (%s)", file.c_str(), [[err localizedDescription] UTF8String]);
		assert(false);
		return NULL;
	}
	
	[player prepareToPlay];
	player.numberOfLoops = 0;

	AudioPlayer *ret = new AudioPlayer();
	
	if (ret != NULL)
		ret->player = player;
	
	return ret;
}

void AudioPlayer::Play()
{
	assert(player != NULL);
	
	[player play];
}

void AudioPlayer::Stop()
{
	assert(player != NULL);
	
	[player stop];
}

void AudioPlayer::SetLoop(bool loop)
{
	player.numberOfLoops = loop ? -1 : 0;
}

void AudioPlayer::SetVolume(float vol)
{
	player.volume = vol;
}

float AudioPlayer::GetVolume() const
{
	return player.volume;
}
