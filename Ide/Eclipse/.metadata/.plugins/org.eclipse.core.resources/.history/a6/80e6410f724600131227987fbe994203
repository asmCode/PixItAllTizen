#include "ImageScrollElement.h"
#include "ImageInfo.h"
#include <Graphics/SpriteBatch.h>
#include <Utils/Image.h>
#include "RefImage.h"
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include "LevelStats.h"
#include "Environment.h"
#include "XMLElement.h"
#include "ClassContainer.h"

ImageScrollElement::ImageScrollElement(ImageInfo *imgInfo,
				   					   const sm::Vec2 &basePos,
									   const sm::Point<float> &fillRectSize,
									   SpriteBatch *sb,
									   const TexPart &bg,
									   const TexPart &padlock,
									   const TexPart &tpResolved,
									   const TexPart &tutRibbon) :
	imgInfo(imgInfo)
{
	resWidth = Environment::GetInstance()->GetScreenWidth();
	resHeight = Environment::GetInstance()->GetScreenHeight();
	
	this->fillRectSize = fillRectSize;
	
	this->basePos = basePos;
	this->shift.Set(0, 0);
	this->sb = sb;
	this->bg = bg;
	this->padlock = padlock;
	this->tpResolved = tpResolved;
	this->tutRibbon = tutRibbon;
	
	this->img = imgInfo->GetRefImageAsTexture();
	
	this->x = basePos.x;
	this->y = 0;
	this->width = bg.ImageRect.Width;
	this->height = bg.ImageRect.Height;
	
	ClassContainer *cc = ClassContainer::GetInstance();
	XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_ImageScrollElement");
	XMLElement *guidefPadlock = guidefPanel->GetSubElement(0);
	XMLElement *guidefTick = guidefPanel->GetSubElement(1);
	XMLElement *guidefTutRibbon = guidefPanel->GetSubElement(2);
	BaseVShift = guidefPanel->GetAttributeInt("base_v_shift");
	
	padlockXDiff = guidefPadlock->GetAttributeInt("posxdiff");
	padlockYDiff = guidefPadlock->GetAttributeInt("posydiff");
	tickXDiff = guidefTick->GetAttributeInt("posxdiff");
	tickYDiff = guidefTick->GetAttributeInt("posydiff");
	margin1 = guidefPanel->GetAttributeInt("margin1");
	margin2 = guidefPanel->GetAttributeInt("margin2");
	tutRibbonXDiff = guidefTutRibbon->GetAttributeInt("posxdiff");
	tutRibbonYDiff = guidefTutRibbon->GetAttributeInt("posydiff");
	
	// calc fill rect
	float dstAspect = fillRectSize.X / fillRectSize.Y;
	float srcAspect = (float)imgInfo->GetWidth() / (float)imgInfo->GetHeight();
	
	if (dstAspect == srcAspect)
	{
		// bg and image have the same aspect
		fillRect = sm::Rect<unsigned>(0, 0, fillRectSize.X, fillRectSize.Y);
	}
	else if (dstAspect < srcAspect)
	{
		// img will be fitted horizontally
		fillRect = sm::Rect<unsigned>(0, 0, fillRectSize.X, (unsigned)(fillRectSize.X / srcAspect));
		fillRect.Y = (fillRectSize.Y - fillRect.Height) / 2; // center vertically
	}
	else if (dstAspect > srcAspect)
	{
		// img will be fitted vertically
		fillRect = sm::Rect<unsigned>(0, 0, (unsigned)(fillRectSize.Y * srcAspect), fillRectSize.Y);
		//fillRect.X = (fillRectSize.X - fillRect.Width) / 2; // center horizontally
	}
}

void ImageScrollElement::SetShift(const sm::Vec2 &shift)
{
	this->shift = shift;
}

void ImageScrollElement::Draw(float time, float seconds)
{
	float xPos = basePos.x + shift.x;
	
	if (xPos > resWidth ||
		xPos + GetTotalWidth() < 0)
		return;
	
	sb->Draw(bg, xPos + fillRect.X - margin1, BaseVShift + fillRect.Y - margin1, fillRect.Width + margin2, fillRect.Height + margin2);
	sb->Draw(img, xPos + fillRect.X, BaseVShift + fillRect.Y, fillRect.Width, fillRect.Height);
	
	if (!imgInfo->GetLevelStats()->IsUnlocked())
	{
		sb->Draw(Color(0, 0, 0, 100), xPos + fillRect.X, BaseVShift + fillRect.Y, fillRect.Width, fillRect.Height);
		sb->Draw(padlock,
				 xPos + fillRect.Width - padlockXDiff,
				 BaseVShift + fillRect.Height - padlockYDiff + ((fillRectSize.Y - fillRect.Height) / 2));
	}
	
	if (imgInfo->GetLevelStats()->GetPoints() > 0)
	{
		sb->Draw(tpResolved,
				 xPos + fillRect.Width - tickXDiff,
				 BaseVShift + fillRect.Height - tickYDiff + ((fillRectSize.Y - fillRect.Height) / 2));
	}
	
	if (imgInfo->IsTutorial())
	{
		sb->Draw(tutRibbon,
				 xPos + GetTotalWidth() - tutRibbon.ImageRect.Width + tutRibbonXDiff,
				 BaseVShift - tutRibbonYDiff);
	}
}

void ImageScrollElement::Update(float time, float seconds)
{
}

float ImageScrollElement::GetBasePos() const
{
	return basePos.x;
}

void ImageScrollElement::SetBasePos(float pos)
{
	basePos.x = pos;
}

ImageInfo *ImageScrollElement::GetImageInfo()
{
	return imgInfo;
}

bool ImageScrollElement::HitTest(int x, int y) const
{	
	float xPos = basePos.x + shift.x;
		
	return
		(x >= xPos && x < xPos + fillRect.Width) &&
		(y >= (fillRect.Y + BaseVShift) && y < (fillRect.Y + fillRect.Height + BaseVShift));
}

int ImageScrollElement::GetTotalWidth() const
{
	return fillRect.Width + 0;
}
