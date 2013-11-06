#include "ImageInfo.h"
#include "RefImage.h"
#include "Palette.h"
#include <GraphicsLibrary/Texture.h>
#include <Utils/Image.h>
#include "LevelStats.h"
#include "ImageMask.h"

#include <GraphicsLibrary/OpenglPort.h>

ImageInfo::ImageInfo(RefImage *refImage,
					 ImageMask *imgMask,
					 const std::string &id,
					 const std::string &title,
					 const std::string &paintDesc,
					 unsigned failLimit,
					 int initialCredits,
					 bool isTutorial)
{
	this->refImage = refImage;
	this->imgMask = imgMask;
	this->id = id;
	this->title = title;
	this->paintDesc = paintDesc;
	this->failLimit = failLimit;
	this->isTutorial = isTutorial;
	this->tex = NULL;
	this->lvlStats = NULL;
	this->initialCredits = initialCredits;
	
	std::vector<Color> colors;
	Palette::GetPaletteColors(refImage, colors);
	this->colorsCount = colors.size();
}

ImageInfo::~ImageInfo()
{
	if (tex != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		delete tex;
	}
}

int ImageInfo::GetWidth() const
{
	return refImage->GetWidth();
}

int ImageInfo::GetHeight() const
{
	return refImage->GetHeight();
}

int ImageInfo::GetColorsCount() const
{
	return colorsCount;
}

std::string ImageInfo::GetId() const
{
	return id;
}

std::string ImageInfo::GetTitle() const
{
	return title;
}

std::string ImageInfo::GetPaintDesc() const
{
	return paintDesc;
}

const RefImage* ImageInfo::GetRefImage() const
{
	return refImage;
}

Texture* ImageInfo::GetRefImageAsTexture()
{
	if (tex == NULL)
	{
		const Image *img = refImage->GetImage();
		
		tex = new Texture(img->GetWidth(), img->GetHeight(), img->GetBpp(), img->GetData());
		tex->BindTexture();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	
	return tex;
}

unsigned ImageInfo::GetFailLimit() const
{
	return failLimit;
}

bool ImageInfo::IsTutorial() const
{
	return isTutorial;
}

void ImageInfo::SetLevelStats(LevelStats *lvlStats)
{
	this->lvlStats = lvlStats;
}

LevelStats *ImageInfo::GetLevelStats()
{
	return lvlStats;
}

ImageMask *ImageInfo::GetImageMask()
{
	return imgMask;
}

int ImageInfo::GetInitialCredits() const
{
	return initialCredits;
}

void ImageInfo::SetInitialCredits(int initialCredits)
{
	this->initialCredits = initialCredits;
}
