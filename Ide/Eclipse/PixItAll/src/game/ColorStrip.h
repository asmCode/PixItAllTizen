#pragma once

#include "Control.h"
#include <GraphicsLibrary/Color.h>
#include <GraphicsLibrary/TexPart.h>
#include <Utils/Math/Matrix.h>
#include <Utils/Math/Vec2.h>

class ColorStrip : public Control
{
private:
	const Color color;
	const unsigned colorIndex;
	const float unrollAngle;
	sm::Vec2 position;
	sm::Matrix trans;
	
	sm::Rect<int> colorStripBounds;
	
	bool peeked;
	bool hoovered;
	float dstAngle;
	TexPart stripTexPart;
	
	float curAngle;
	float timeDelay;
	float unrollTime;
	float rollTime;
	sm::Vec2 rollPosition;
	sm::Vec2 curPosition;
	
	bool rolling;
	bool isRolled;
	
	float stripShiftValue;

	// hoover / peek purpose
	float advancedCur;
	float advancedDst;

	bool HitTest(int x, int y) const;
	void OnDraw(float time, float ms);
	void OnUpdate(float time, float ms);
	
public:
	ColorStrip(const Color &color,
			   unsigned colorIndex,
			   const sm::Vec2 &position,
			   TexPart stripTexPart,
			   sm::Rect<int> colorStripBounds,
			   float angle,
			   float timeDelay);
	
	void Unroll();
	void Roll();
	bool HitTest2(int x, int y, float &accuracy) const;
	Color GetColor() const;
	unsigned GetColorIndex() const;
	void SetPeeked(bool peeked);
	void SetHoovered(bool val);
	
	bool IsRolled() const;
};
