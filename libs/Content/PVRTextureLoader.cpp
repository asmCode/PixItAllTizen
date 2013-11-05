#import "PVRTextureLoader.h"
#import "../Graphics/PVRTexture.h"
#import <fstream>
#import <vector>
#import <string>
#import <algorithm>
#import <CoreFoundation/CFByteOrder.h>

#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff

static char gPVRTexIdentifier[] = "PVR!";

enum
{
	kPVRTextureFlagTypePVRTC_2 = 24,
	kPVRTextureFlagTypePVRTC_4
};

typedef struct _PVRTexHeader
{
	unsigned headerLength;
	unsigned height;
	unsigned width;
	unsigned numMipmaps;
	unsigned flags;
	unsigned dataLength;
	unsigned bpp;
	unsigned bitmaskRed;
	unsigned bitmaskGreen;
	unsigned bitmaskBlue;
	unsigned bitmaskAlpha;
	unsigned pvrTag;
	unsigned numSurfs;
} PVRTexHeader;

char *PVRTextureLoader::GetFileData(const char *path)
{
	std::ifstream file(path, std::ios_base::binary);
	if (file.fail())
		return NULL;
	
	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	
	char *buff = new char[fileSize];
	if (buff == NULL)
		return NULL;
	
	file.read(buff, fileSize);
	file.close();
	
	return buff;
}

void * PVRTextureLoader::Load(const char *path, sm::IServiceProvider *serviceProvider)
{	
	bool success = false;
	PVRTexHeader *header = NULL;
	unsigned flags, pvrTag;
	unsigned dataLength = 0, dataOffset = 0, dataSize = 0;
	unsigned blockSize = 0, widthBlocks = 0, heightBlocks = 0;
	unsigned width = 0, height = 0, bpp = 4;
	unsigned baseWidth = 0, baseHeight = 0;
	unsigned formatFlags;
	GLenum internalFormat;
	std::vector<std::string> texData;
	bool hasAlpha = false;
	
	char *data = GetFileData(path);
	if (data == NULL)
		return NULL;
	
	header = (PVRTexHeader *)data;
	
	pvrTag = CFSwapInt32LittleToHost(header ->pvrTag);

	if (gPVRTexIdentifier[0] != ((pvrTag >>  0) & 0xff) ||
		gPVRTexIdentifier[1] != ((pvrTag >>  8) & 0xff) ||
		gPVRTexIdentifier[2] != ((pvrTag >> 16) & 0xff) ||
		gPVRTexIdentifier[3] != ((pvrTag >> 24) & 0xff))
	{
		return NULL;
	}
	
	flags = CFSwapInt32LittleToHost(header->flags);
	formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;
	
	if (formatFlags == kPVRTextureFlagTypePVRTC_4 || formatFlags == kPVRTextureFlagTypePVRTC_2)
	{
		if (formatFlags == kPVRTextureFlagTypePVRTC_4)
			internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		else if (formatFlags == kPVRTextureFlagTypePVRTC_2)
			internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
	
		baseWidth = width = CFSwapInt32LittleToHost(header ->width);
		baseHeight = height = CFSwapInt32LittleToHost(header ->height);
		
		header->bitmaskAlpha ? hasAlpha = true : hasAlpha = false;
		
		dataLength = CFSwapInt32LittleToHost(header ->dataLength);
		
		char *bytes = data + sizeof(PVRTexHeader);
		
		// Calculate the data size for each texture level and respect the minimum number of blocks
		while (dataOffset < dataLength)
		{
			if (formatFlags == kPVRTextureFlagTypePVRTC_4)
			{
				blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
				widthBlocks = width / 4;
				heightBlocks = height / 4;
				bpp = 4;
			}
			else
			{
				blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
				widthBlocks = width / 8;
				heightBlocks = height / 4;
				bpp = 2;
			}
			
			// Clamp to minimum number of blocks
			if (widthBlocks < 2)
				widthBlocks = 2;
			if (heightBlocks < 2)
				heightBlocks = 2;

			dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
			
			texData.push_back(std::string(bytes + dataOffset, dataSize));
			
			dataOffset += dataSize;
			
			width = std::max(width >> 1, (unsigned)1);
			height = std::max(height >> 1, (unsigned)1);
		}
				  
		success = true;
	}
	
	delete [] data;
	
	return new PVRTexture(baseWidth, baseHeight, bpp, internalFormat, texData);
}
