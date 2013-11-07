#pragma once

#include <GraphicsLibrary/IUpdateable.h>
#include <GraphicsLibrary/IDrawable.h>
#include <GraphicsLibrary/TexPart.h>
#include <GraphicsLibrary/Color.h>
#include "ClassContainer.h"
#include <Utils/Math/Matrix.h>

class SpriteBatch;

class FailPanel :
	public IUpdateable,
	public IDrawable
{
private:
	FailPanel();
	
	TexPart failIcon;
	
	unsigned failLimit;
	unsigned failCount;
	
	const Color activeIconColor;
	const Color inactiveIconColor;
		
	SpriteBatch *sb;
	
public:
	static FailPanel *Create(SpriteBatch *sb, unsigned failLimit);
	
	void Update(float time, float ms);
	void Draw(float time, float ms);
	
	void ResetFails();
	void MakeFail();
};
