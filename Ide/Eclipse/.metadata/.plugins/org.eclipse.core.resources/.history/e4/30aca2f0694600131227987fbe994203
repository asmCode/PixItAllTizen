#include "AlAudioPlayer.h"
#include <CoreFoundation/CFURL.h>
#include <AudioToolbox/ExtendedAudioFile.h>
#include "Log.h"

alBufferDataStaticProcPtr alBufferDataStatic;

ALCdevice *AlAudioPlayer::audioDev;
ALCcontext *AlAudioPlayer::audioContext;
bool AlAudioPlayer::isInitialized;

AlAudioPlayer::AlAudioPlayer()
{
	sourceName = 0;
	bufferName = 0;
	audioData = NULL;
}

AlAudioPlayer::~AlAudioPlayer()
{
	if (sourceName != 0)
	{
		alDeleteSources(1, &sourceName);
		sourceName = 0;
	}
	
	if (bufferName != 0)
	{
		alDeleteBuffers(1, &bufferName);
		bufferName = 0;
	}
	
	if (audioData != NULL)
	{
		delete [] audioData;
		audioData = NULL;
	}
}

bool AlAudioPlayer::IsInitialized()
{
	return isInitialized;
}

bool AlAudioPlayer::Initialize()
{
	if (isInitialized)
		return true;
	
	audioDev = alcOpenDevice(NULL);
	assert(audioDev != NULL);
	if (audioDev == NULL)
	{
		Deinitialize();
		return false;
	}
	
	audioContext = alcCreateContext(audioDev, NULL);
	assert(audioContext != NULL);
	if (audioContext == NULL)
	{
		Deinitialize();
		return false;
	}
	
	if (alcMakeContextCurrent(audioContext) != AL_TRUE)
	{
		assert(false);
		Deinitialize();
		return false;
	}
	
	alBufferDataStatic = (alBufferDataStaticProcPtr)alcGetProcAddress(audioDev, "alBufferDataStatic");
	
	isInitialized = true;
	
	return true;
}

void AlAudioPlayer::Deinitialize()
{
	if (audioContext != NULL)
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(audioContext);
		audioContext = NULL;
	}
	
	if (audioDev != NULL)		
	{
		alcCloseDevice(audioDev);
		audioDev = NULL;
	}
	
	isInitialized = false;
}

AlAudioPlayer* AlAudioPlayer::LoadFromFile(const std::string &file, bool stereo)
{
	CFURLRef url = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8 *)file.c_str(), file.size(), false);
	
	OSStatus res;
	
	ExtAudioFileRef audioFile = 0;
	res = ExtAudioFileOpenURL(url, &audioFile);
	
	AudioStreamBasicDescription fileDataFormat;
	SInt64 fileLengthFrames = 0;
	UInt32 fileLengthFramesSize = sizeof(SInt64);
	UInt32 fileDataFormatSize = sizeof(AudioStreamBasicDescription);
	res = ExtAudioFileGetProperty(audioFile, kExtAudioFileProperty_FileDataFormat, &fileDataFormatSize, &fileDataFormat);
	res = ExtAudioFileGetProperty(audioFile, kExtAudioFileProperty_FileLengthFrames, &fileLengthFramesSize, &fileLengthFrames);
	
	AudioStreamBasicDescription outputFormat;
	outputFormat.mSampleRate = fileDataFormat.mSampleRate;
    outputFormat.mChannelsPerFrame = stereo ? 2 : 1;
	outputFormat.mFormatID = kAudioFormatLinearPCM;
    outputFormat.mBytesPerPacket = 2 * outputFormat.mChannelsPerFrame;
    outputFormat.mFramesPerPacket = 1;
    outputFormat.mBytesPerFrame = 2 * outputFormat.mChannelsPerFrame;
    outputFormat.mBitsPerChannel = 16;
    outputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
	ExtAudioFileSetProperty(audioFile, kExtAudioFileProperty_ClientDataFormat, sizeof(AudioStreamBasicDescription), &outputFormat);
	
	unsigned int audioDataSize = fileLengthFrames * outputFormat.mBytesPerFrame;
	char *audioData = new char[audioDataSize];
	
	AudioBufferList audioBufferList;
	audioBufferList.mNumberBuffers = 1;
	audioBufferList.mBuffers[0].mNumberChannels = stereo ? 2 : 1;
	audioBufferList.mBuffers[0].mDataByteSize = audioDataSize;
	audioBufferList.mBuffers[0].mData = audioData;
	
	UInt32 ioNumberFrames = fileLengthFrames;
	res = ExtAudioFileRead(audioFile, &ioNumberFrames, &audioBufferList);
	
	AlAudioPlayer *player = new AlAudioPlayer();
	player->audioData = audioData;
	
	alGenBuffers(1, &player->bufferName);
	//alBufferData(player->bufferName, AL_FORMAT_STEREO16, audioData, audioDataSize, 44100/2);
	alBufferDataStatic(player->bufferName, stereo ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, audioData, audioDataSize, 44100);
	
	alGenSources(1, &player->sourceName);
	alSourcei(player->sourceName, AL_BUFFER, player->bufferName);
							
	ExtAudioFileDispose(audioFile);
	CFRelease(url);
										   	
	return player;
}

void AlAudioPlayer::Play()
{
	alSourcePlay(sourceName);
}

void AlAudioPlayer::Stop()
{
	alSourceStop(sourceName);
}

void AlAudioPlayer::SetLoop(bool loop)
{
	alSourcei(sourceName, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void AlAudioPlayer::SetVolume(float vol)
{
	alSourcef(sourceName, AL_GAIN, (ALfloat)vol);
}

float AlAudioPlayer::GetVolume() const
{
	float vol;
	alGetSourcef(sourceName, AL_GAIN, &vol);
	return vol;
}
