#pragma once

#include <GraphicsLibrary/IDrawable.h>
#include <GraphicsLibrary/IUpdateable.h>
#include <GraphicsLibrary/TexPart.h>
#include <Utils/Math/Vec2.h>
#include "Control.h"

class ImageInfo;
class SpriteBatch;

class ImageScrollElement:
	public Control
{
private:
	unsigned BaseVShift;
	
	ImageInfo *imgInfo;
	sm::Vec2 basePos;
	sm::Vec2 shift;
	TexPart bg;
	TexPart padlock;
	TexPart tpResolved;
	TexPart tutRibbon;
	Texture *img;
	SpriteBatch *sb;
	
	unsigned padlockXDiff;
	unsigned padlockYDiff;
	unsigned tickXDiff;
	unsigned tickYDiff;
	unsigned tutRibbonXDiff;
	unsigned tutRibbonYDiff;
	unsigned margin1;
	unsigned margin2;
	
	unsigned resWidth;
	unsigned resHeight;
	
	sm::Rect<unsigned> fillRect;
	sm::Point<float> fillRectSize;
	
	bool HitTest(int x, int y) const;
	
public:
	ImageScrollElement(ImageInfo *imgInfo,
					   const sm::Vec2 &basePos,
					   const sm::Point<float> &fillRectSize,
					   SpriteBatch *sb,
					   const TexPart &bg,
					   const TexPart &padlock,
					   const TexPart &tpResolved,
					   const TexPart &tutRibbon);
	
	void SetShift(const sm::Vec2 &shift);
	void Draw(float time, float seconds);
	void Update(float time, float seconds);
	float GetBasePos() const;
	void SetBasePos(float pos);
	ImageInfo *GetImageInfo();
	
	int GetTotalWidth() const;
};
