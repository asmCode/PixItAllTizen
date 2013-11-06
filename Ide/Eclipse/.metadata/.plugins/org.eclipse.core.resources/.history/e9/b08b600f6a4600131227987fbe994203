#pragma once

#include <string>
#include <list>
#include "ImageInfo.h"

class ImageMask;

class ImagesCollection
{
private:
	std::list<ImageInfo*> images;
	
	ImagesCollection();
	
	static ImageMask* LoadImageMask(const std::string &imageMaskPath);
	static void ProcessImageInfoAfterLoad(ImageInfo *imgInfo);
	
public:
	static const int UnlockedNeverPlayed = 12; //TEST - how many levels that hasn't been played (0 points) are unlocked
	
	~ImagesCollection();
	
	static ImagesCollection* Load(const std::string &path,
								  const std::string &filename);
	
	std::list<ImageInfo*>& GetImages();
	ImageInfo *GetNextImage(const ImageInfo* imgInfo);
	
	// Get next unresolved and unlocked image after imgInfo. If end was reached, then
	// images are searched from the beginning (except tutorial images)
	ImageInfo *GetNextUnresolvedImage(const ImageInfo* imgInfo);
	
	// methods used after user login
	void LoadLevelStats(const std::string &levelsStatsPath);
	unsigned GetTotalPoints();
	unsigned GetUnlockedAndNeverPlayedCount();
	unsigned GetFinishedLevelsCount();
	void UnlockLevels();
};
