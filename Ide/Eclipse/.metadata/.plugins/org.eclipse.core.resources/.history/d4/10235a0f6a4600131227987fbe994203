#pragma once

#include <string>

class RefImage;
class Texture;
class LevelStats;
class ImageMask;

class ImageInfo
{
private:
	RefImage *refImage;
	ImageMask *imgMask;
	int colorsCount;
	Texture *tex;
	
	LevelStats *lvlStats;
	
	std::string id;
	std::string	title;
	std::string paintDesc;
	unsigned	failLimit;
	bool		isTutorial;
	int			initialCredits; // if -1, then set auto (based on image size)

public:
	ImageInfo(RefImage *refImage,
			  ImageMask *imgMask,
			  const std::string &id,
			  const std::string &title,
			  const std::string &paintDesc,
			  unsigned failLimit,
			  int initialCredits,
			  bool isTutorial);
	~ImageInfo();
	
	int GetWidth() const;
	int GetHeight() const;
	int GetColorsCount() const;
	std::string GetId() const;
	std::string GetTitle() const;
	std::string GetPaintDesc() const;
	unsigned GetFailLimit() const;
	bool IsTutorial() const;
	
	int GetInitialCredits() const;
	void SetInitialCredits(int initialCredits);
	
	const RefImage* GetRefImage() const;
	ImageMask *GetImageMask();
	Texture* GetRefImageAsTexture();
	
	void SetLevelStats(LevelStats *lvlStats);
	LevelStats *GetLevelStats();
};
