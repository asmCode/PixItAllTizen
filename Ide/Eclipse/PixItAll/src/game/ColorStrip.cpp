#include "ColorStrip.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include <Utils/Math/Matrix.h>
#include <Utils/Utils/Randomizer.h>
#include "Environment.h"
#include "XMLElement.h"
#include "ClassContainer.h"

ColorStrip::ColorStrip(const Color &color,
					   unsigned colorIndex,
					   const sm::Vec2 &position,
					   TexPart stripTexPart,
					   sm::Rect<int> colorStripBounds,
					   float angle,
					   float timeDelay) :
color(color),
colorIndex(colorIndex),
unrollAngle(angle),
position(position)
{	
	this ->curAngle = 0.0f;
	this ->dstAngle = 0.1f + 0.05f * unrollAngle;
	this ->dstAngle = 0.0f;
	this->timeDelay = timeDelay;
	this->unrollTime = 0;
	this->rolling = false;
	this->isRolled = true;
	this->curPosition = position;
	
	this->advancedCur = 0.0f;
	this->advancedDst = 0.0f;
	
	width = colorStripBounds.Width;
	height = colorStripBounds.Height;
	
	this->rollPosition = sm::Vec2(position.x, PixItAll::Environment::GetInstance()->GetScreenHeight() + colorStripBounds.Width);
	
	this ->colorStripBounds = colorStripBounds;
	
	this ->stripTexPart = stripTexPart;
	peeked = false;
	hoovered = false;
	
	ClassContainer *cc = ClassContainer::GetInstance();
	XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_ColorStripPanel");
	XMLElement *guidefColorStrip = guidefPanel->GetSubElement(0);
	
	stripShiftValue = guidefColorStrip->GetAttributeInt("strip_shift");
}

bool ColorStrip::HitTest(int x, int y) const
{
	return false;
}

unsigned ColorStrip::GetColorIndex() const
{
	return colorIndex;
}

Color ColorStrip::GetColor() const
{
	return color;
}

void ColorStrip::SetPeeked(bool peeked)
{
	this ->peeked = peeked;

	advancedDst = peeked || hoovered ? stripShiftValue : 0.0f;
}

void ColorStrip::SetHoovered(bool val)
{
	this ->hoovered = val;
	
	advancedDst = val || peeked ? stripShiftValue : 0.0f;
}

bool ColorStrip::HitTest2(int x, int y, float &accuracy) const
{
	sm::Matrix trans =
		sm::Matrix::TranslateMatrix(curPosition.x, curPosition.y + 15, 0) *
		sm::Matrix::RotateAxisMatrix(curAngle, sm::Vec3(0, 0, 1)) *
		sm::Matrix::TranslateMatrix(colorStripBounds.Width / 2, 0, 0) *
		sm::Matrix::ScaleMatrix(colorStripBounds.Width, colorStripBounds.Height, 1);
	
	sm::Matrix transInv = trans.GetInversed();
	sm::Vec3 point = transInv * sm::Vec3(x, y, 0.0f);
	
	//printf("color strip hit test: (%.2f, %.2f)", point.x, point.y);
	
	//if ((point.x >= 0.1f && point.x <= 0.44f) &&
	if ((point.x >= -0.20f && point.x <= 0.44f) &&
		(point.y >= -0.4f && point.y <= 0.4f))
	{
		accuracy = fabsf(point.y);
		//printf(" hit!\n");
		return true;
	}
	
	//printf("\n");
	return false;
}

void ColorStrip::OnUpdate(float time, float ms)
{
	if (isRolled)
		return;
	
	if (advancedCur != advancedDst)
	{
		advancedCur += (advancedDst - advancedCur) * ms * 10;
		if (fabs(advancedCur - advancedDst) < 2)
			advancedCur = advancedDst;
	}
	
	if (rolling)
	{
		rollTime += ms;
	
		if (curAngle != dstAngle)
		{
			curAngle += (dstAngle - curAngle ) * ms * 12;
			
			if (fabs(curAngle - dstAngle) < 0.01f)
				curAngle = dstAngle;
		}
		
		//if (curAngle == dstAngle)
		if (rollTime > 0.3f)
			curPosition += (rollPosition - curPosition) * ms * 10;
		
		if (rollTime > 1.0f)
			isRolled = true;
	}
	else
	{
		
		//if (curAngle == dstAngle)
		//		return;
		
		unrollTime += ms;
		if (unrollTime <= timeDelay)
			return;
		
		if (curAngle != dstAngle)
			curAngle += (dstAngle - curAngle ) * ms * 8;
		
		if (fabs(curAngle - dstAngle) < 0.001f)
			curAngle = dstAngle;
	}
	
	sm::Vec2 peekedShift(0, 0);
	if (peeked || hoovered)
		peekedShift.x = stripShiftValue;
	
	trans =
		sm::Matrix::TranslateMatrix(curPosition.x, curPosition.y + 15, 0) *
		sm::Matrix::RotateAxisMatrix(curAngle, sm::Vec3(0, 0, 1)) *
		sm::Matrix::TranslateMatrix(colorStripBounds.Width / 2 + advancedCur, 0, 0) *
		sm::Matrix::ScaleMatrix(colorStripBounds.Width, colorStripBounds.Height, 1);
}

void ColorStrip::OnDraw(float time, float ms)
{
	if (isRolled)
		return;

	if (curAngle == 0)
		return;
	
	spriteBatch ->Draw(stripTexPart, color, trans);
	//spriteBatch ->Draw(color, trans);
}

void ColorStrip::Unroll()
{
	curAngle = 0.0f;
	dstAngle = unrollAngle;
	curPosition = position;
	unrollTime = 0.0f;
	rolling = false;
	isRolled = false;
}

void ColorStrip::Roll()
{
	rollTime = 0.0f;
	rolling = true;
	dstAngle = -3.14159265f / 2.0f;
	//position.y = 100.0f;
}

bool ColorStrip::IsRolled() const
{
	return isRolled;
}
