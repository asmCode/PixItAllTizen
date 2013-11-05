#include "AqAudioPlayer.h"

void DeriveBufferSize(AudioStreamBasicDescription &ASBDesc,
					  UInt32 maxPacketSize,
					  Float64 seconds,
					  UInt32 *outBufferSize,
					  UInt32 *outNumPacketsToRead)
{	
    static const int maxBufferSize = 0x50000;
    static const int minBufferSize = 0x4000;
    
	if (ASBDesc.mFramesPerPacket != 0)
	{
        Float64 numPacketsForTime = ASBDesc.mSampleRate / ASBDesc.mFramesPerPacket * seconds;
        *outBufferSize = numPacketsForTime * maxPacketSize;
		
    }
	else
	{
		*outBufferSize = maxBufferSize > maxPacketSize ? maxBufferSize : maxPacketSize;
    }
	
    if (*outBufferSize > maxBufferSize &&
        *outBufferSize > maxPacketSize)		
        *outBufferSize = maxBufferSize;
    else
	{
        if (*outBufferSize < minBufferSize)
            *outBufferSize = minBufferSize;
    }
	
    *outNumPacketsToRead = *outBufferSize / maxPacketSize;
}

AqAudioPlayer* AqAudioPlayer::LoadFromFile(const std::string &file)
{
	AqAudioPlayer *player = new AqAudioPlayer();
	
	CFURLRef url = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8 *)file.c_str(), file.size(), false);
	
	OSStatus result = AudioFileOpenURL(url, kAudioFileReadPermission, 0, &player->mAudioFile);
	
	UInt32 dataFormatSize = sizeof(player->mDataFormat);
	result = AudioFileGetProperty(player->mAudioFile, kAudioFilePropertyDataFormat, &dataFormatSize, &player->mDataFormat);
	
	result = AudioQueueNewOutput(&player->mDataFormat,
								 AqAudioPlayer::HandleOutputBuffer,
								 player,
								 CFRunLoopGetCurrent(),
								 kCFRunLoopCommonModes,
								 0,
								 &player->mQueue);
	
	UInt32 maxPacketSize;
	UInt32 propertySize = sizeof(maxPacketSize);
	
	AudioFileGetProperty(player->mAudioFile,
						 kAudioFilePropertyPacketSizeUpperBound,
						 &propertySize,
						 &maxPacketSize);
	
	DeriveBufferSize(player->mDataFormat,
					 maxPacketSize,
					 0.5,
					 &player->bufferByteSize,
					 &player->mNumPacketsToRead);
	
	bool isFormatVBR = player->mDataFormat.mBytesPerPacket == 0 ||
					   player->mDataFormat.mFramesPerPacket == 0;
	
	if (isFormatVBR)
		player->mPacketDescs = (AudioStreamPacketDescription*)malloc(player->mNumPacketsToRead * sizeof(AudioStreamPacketDescription));
	else
		player->mPacketDescs = NULL;
	
	player->mCurrentPacket = 0;
	
	player->mIsRunning = true;
	
	for (int i = 0; i < kNumberBuffers; ++i)
	{	
		AudioQueueAllocateBuffer(player->mQueue, player->bufferByteSize, &player->mBuffers[i]);
		HandleOutputBuffer(player, player->mQueue, player->mBuffers[i]);
	}
	
	player->mIsRunning = false;

	CFRelease(url);
	
	return player;
}

void AqAudioPlayer::HandleOutputBuffer(void *aqData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{
	AqAudioPlayer *_this = (AqAudioPlayer*)aqData;
	
    if (_this->mIsRunning == 0)
		return;
	
    UInt32 numBytesReadFromFile;
    UInt32 numPackets = _this->mNumPacketsToRead;
    AudioFileReadPackets (_this->mAudioFile,
						  false,
						  &numBytesReadFromFile,
						  _this->mPacketDescs, 
						  _this->mCurrentPacket,
						  &numPackets,
						  inBuffer->mAudioData);
	
    if (numPackets > 0)
	{
        inBuffer->mAudioDataByteSize = numBytesReadFromFile;
		AudioQueueEnqueueBuffer (_this->mQueue,
								 inBuffer,
								 (_this->mPacketDescs ? numPackets : 0),
								 _this->mPacketDescs);
        _this->mCurrentPacket += numPackets;
    }
	else
	{
		if (_this->loop)
		{
			_this->mCurrentPacket = 0;
			HandleOutputBuffer(aqData, inAQ, inBuffer);
		}
		else
		{
			_this->Stop();
		}
    }
}

AqAudioPlayer::AqAudioPlayer()
{
	mQueue = NULL;
	mAudioFile = NULL;
	mPacketDescs = NULL;
}

AqAudioPlayer::~AqAudioPlayer()
{
	if (mQueue != NULL)
		AudioQueueDispose(mQueue, true);
	
	if (mAudioFile != NULL)
		AudioFileClose(mAudioFile);
	
	if (mPacketDescs != NULL)
		free(mPacketDescs);
}

void AqAudioPlayer::Play()
{	
	mIsRunning = true;
	AudioQueueStart(mQueue, NULL);
}

void AqAudioPlayer::Stop()
{
	mIsRunning = false;
	AudioQueueStop(mQueue, false);
}

void AqAudioPlayer::SetLoop(bool loop)
{
	this->loop = loop;
}

void AqAudioPlayer::SetVolume(float vol)
{
	AudioQueueSetParameter(mQueue, kAudioQueueParam_Volume, vol);
}

float AqAudioPlayer::GetVolume() const
{
	float vol;
	AudioQueueGetParameter(mQueue, kAudioQueueParam_Volume, &vol);
	return vol;
}
