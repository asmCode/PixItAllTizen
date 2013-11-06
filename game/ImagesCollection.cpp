#include "ImagesCollection.h"
#include <stddef.h>
#include <Utils/Image.h>
#include "RefImage.h"
#include "LevelStats.h"
#include "ImageMask.h"
#include <Utils/Utils/Path.h>
#include <Utils/Log.h>
#include <math.h>

#define TIXML_USE_STL
#include <Utils/Xml/tinyxml.h>

ImagesCollection::ImagesCollection()
{
}

ImagesCollection::~ImagesCollection()
{
	std::list<ImageInfo*>::iterator it;
	for (it = images.begin(); it != images.end(); it++)
		delete (*it);
}

ImagesCollection* ImagesCollection::Load(const std::string &path,
										 const std::string &filename)
{	
	std::list<ImageInfo*> images;
	
	TiXmlDocument xmlDoc;
	if (!xmlDoc.LoadFile(path + filename))
		return NULL;
	
	const TiXmlElement *rootNode = xmlDoc.RootElement();
	if (rootNode == NULL || (strcmp(rootNode ->Value(), "images") != 0))
		return NULL;
	
	const TiXmlNode *child = NULL;
	while((child = rootNode->IterateChildren(child)) != NULL)
	{
		const TiXmlElement *charElement = child ->ToElement();
		if (strcmp(charElement ->Value(), "image") == 0)
		{
			std::string id;
			std::string fileSource;
			std::string title;
			std::string paintDesc;
			std::string failLimitStr;
			std::string initialCreditsStr;			
			unsigned failLimit;
			int initialCredits = -1;
			if (charElement ->QueryStringAttribute("file_source", &fileSource) != TIXML_SUCCESS ||
				charElement ->QueryStringAttribute("id", &id) != TIXML_SUCCESS ||
				charElement ->QueryStringAttribute("title", &title) != TIXML_SUCCESS ||
				charElement ->QueryStringAttribute("fail_limit", &failLimitStr) != TIXML_SUCCESS)
				continue;
			
			// TEST - skipping test images
			if (id.size() >= 5 &&
				id.c_str()[0] == 't' &&
				id.c_str()[1] == 'e' &&
				id.c_str()[2] == 's' &&
				id.c_str()[3] == 't' &&
				id.c_str()[4] == '_')
				continue;
			
			charElement ->QueryStringAttribute("paint_desc", &paintDesc);
			charElement ->QueryStringAttribute("initial_credits", &initialCreditsStr);
			
			int isTutorial = 0;
			charElement ->QueryIntAttribute("tutorial", &isTutorial);
			
			sscanf(failLimitStr.c_str(), "%u", &failLimit);
			
			if (!failLimitStr.empty())
				sscanf(initialCreditsStr.c_str(), "%u", &initialCredits);
			
			Path imgPath((path + fileSource).c_str());
			Image *img = Image::LoadImage(imgPath.GetFullPath().c_str());
			if (img == NULL)
			{
				Log::LogE("Couldn't load image '%s'", imgPath.GetFullPath().c_str());
				return NULL;
			}
			
			RefImage *refImg = RefImage::Create(img);
			if (refImg == NULL)
				return NULL;
			
			ImageMask *imgMask = LoadImageMask(imgPath.GetPath() + imgPath.GetFilename() + ".msk");

			ImageInfo *imgInfo = new ImageInfo(refImg, imgMask, id, title, paintDesc, failLimit, initialCredits, isTutorial == 1);
			if (imgInfo == NULL)
				return NULL;
			
			ProcessImageInfoAfterLoad(imgInfo);
						
			images.push_back(imgInfo);
		}
	}
	
	ImagesCollection *ret = new ImagesCollection();
	if (ret == NULL)
		return NULL;
	
	ret->images = images;
	return ret;
}

std::list<ImageInfo*>& ImagesCollection::GetImages()
{
	return images;
}

ImageInfo *ImagesCollection::GetNextImage(const ImageInfo* imgInfo)
{
	std::list<ImageInfo*>::iterator it;
	std::list<ImageInfo*>::iterator nextIt = images.end();
	for (it = images.begin(); it != images.end(); it++)
	{
		if (*it == imgInfo)
		{
			nextIt = it;
			nextIt++;
		}
	}
	
	if (nextIt != images.end())
		return *nextIt;
	else
		return NULL;
}

ImageInfo *ImagesCollection::GetNextUnresolvedImage(const ImageInfo* imgInfo)
{
	bool afterCurrent = false;
	
	std::list<ImageInfo*>::iterator it;
	for (it = images.begin(); it != images.end(); it++)
	{
		if (!afterCurrent && (*it == imgInfo))
		{
			afterCurrent = true;
			continue;
		}
		
		if (afterCurrent)
		{
			if ((*it)->GetLevelStats()->GetPoints() == 0 &&
				(*it)->GetLevelStats()->IsUnlocked() &&
				!(*it)->IsTutorial())
				return (*it);
		}
	}
	
	// if we didn't already search from begining, then do that
	if (images.size() > 0 && images.front() != imgInfo)
		return GetNextUnresolvedImage(images.front());
	
	return NULL;
}

unsigned ImagesCollection::GetTotalPoints()
{
	unsigned points = 0;
	
	std::list<ImageInfo*>::iterator it;
	for (it = images.begin(); it != images.end(); it++)
	{
		LevelStats *lvlStats = (*it)->GetLevelStats();
		if (lvlStats == NULL)
			continue;
		
		points += lvlStats->GetPoints();
	}
	
	return points;
}

unsigned ImagesCollection::GetUnlockedAndNeverPlayedCount()
{
	int count = 0;
	
	std::list<ImageInfo*>::iterator it;
	for (it = images.begin(); it != images.end(); it++)
	{
		LevelStats *lvlStats = (*it)->GetLevelStats();
		if (lvlStats == NULL)
			continue;
		
		if (lvlStats->GetPoints() == 0 && lvlStats->IsUnlocked())
			count++;
	}
	
	return count;
}

void ImagesCollection::UnlockLevels()
{
	unsigned unlockedAndNeverPlayedCount = GetUnlockedAndNeverPlayedCount();
	
	if (unlockedAndNeverPlayedCount >= UnlockedNeverPlayed)
	{
		//assert(unlockedAndNeverPlayedCount == UnlockedNeverPlayed);
		return;
	}
	
	unsigned toUnlockLeft = UnlockedNeverPlayed - unlockedAndNeverPlayedCount;
	
	std::list<ImageInfo*>::iterator it;
	for (it = images.begin(); it != images.end() && toUnlockLeft > 0; it++)
	{
		LevelStats *lvlStats = (*it)->GetLevelStats();
		if (lvlStats == NULL)
			continue;
		
		if (lvlStats->GetPoints() == 0 && !lvlStats->IsUnlocked())
		{
			lvlStats->SetLock(false);
			lvlStats->Save();
			toUnlockLeft--;
		}
	}	
}

void ImagesCollection::LoadLevelStats(const std::string &levelsStatsPath)
{
	std::list<ImageInfo*>::iterator it;
	for (it = images.begin(); it != images.end(); it++)
	{
		LevelStats *lvlStats = new LevelStats(levelsStatsPath + (*it)->GetId() + ".stats");
		if (!lvlStats->Load())
		{
			assert(false);
		}
		(*it)->SetLevelStats(lvlStats);
	}
}

ImageMask* ImagesCollection::LoadImageMask(const std::string &imageMaskPath)
{	
	if (!Path::IsFileExists(imageMaskPath.c_str()))
		return NULL;
	
	ImageMask *imgMask = new ImageMask();
	if (!imgMask->Load(imageMaskPath))
	{
		delete imgMask;
		imgMask = NULL;
	}
	
	return imgMask;
}

void ImagesCollection::ProcessImageInfoAfterLoad(ImageInfo *imgInfo)
{
	if (imgInfo->GetInitialCredits() == -1)
	{
		int imgComplex = (int)sqrtf(imgInfo->GetWidth() * imgInfo->GetHeight());
		if (imgComplex > 60)
			imgInfo->SetInitialCredits(60);
		else if (imgComplex > 40)
			imgInfo->SetInitialCredits(60);
		else if (imgComplex > 20)
			imgInfo->SetInitialCredits(30);
		else if (imgComplex > 0)
			imgInfo->SetInitialCredits(15);
	}
}

unsigned ImagesCollection::GetFinishedLevelsCount()
{
	unsigned count = 0;
	
	std::list<ImageInfo*>::iterator it;
	for (it = images.begin(); it != images.end(); it++)
	{
		LevelStats *lvlStats = (*it)->GetLevelStats();
		if (lvlStats == NULL)
			continue;
		
		if (lvlStats->GetPoints() > 0)
			count++;
	}
	
	return count;
}
