#pragma once

#include <AudioToolbox/AudioQueue.h>
#include <AudioToolbox/AudioFile.h>
#include "IAudioPlayer.h"
#include <string>

class AqAudioPlayer : public IAudioPlayer
{
private:
	static void HandleOutputBuffer(void *aqData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);
	
	static const int kNumberBuffers = 3;
	
	AudioStreamBasicDescription   mDataFormat;	
    AudioQueueRef                 mQueue;
    AudioQueueBufferRef           mBuffers[kNumberBuffers];
    AudioFileID                   mAudioFile;
    UInt32                        bufferByteSize;
    SInt64                        mCurrentPacket;
    UInt32                        mNumPacketsToRead;
    AudioStreamPacketDescription  *mPacketDescs;
    bool                          mIsRunning;
	
	bool							loop;

public:
	AqAudioPlayer();
	~AqAudioPlayer();
	
	static AqAudioPlayer* LoadFromFile(const std::string &file);
	
	void Play();
	void Stop();
	
	void SetLoop(bool loop);
	
	void SetVolume(float vol);
	float GetVolume() const;
};
