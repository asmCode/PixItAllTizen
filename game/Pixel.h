#pragma once

#include <GraphicsLibrary/IUpdateable.h>
#include <GraphicsLibrary/IDrawable.h>
#include <GraphicsLibrary/Color.h>
#include <GraphicsLibrary/TexPart.h>
#include <Utils/Math/Vec2.h>
#include <Utils/Math/Matrix.h>

class SpriteAnim;
class SpriteBatch;
class Texture;

class Pixel : public IUpdateable, public IDrawable
{
private:
	Color color;
	SpriteBatch *spriteBatch;
	SpriteAnim *spriteAnim;
	TexPart pixelFrame;

	sm::Matrix colorTrans;
	sm::Matrix frameTrans;

	const int baseX;
	const int baseY;
		
	static const float CorrectAnimCooldown;
	float correctAnimProgress;
	
	float colorFloodTime;
	float colorFloodValue;
	sm::Vec2 mirrorAndFlip;
	
	bool finished;
	
public:
	static const float ColorFloodDuration;
	static const int BaseWidth = 80;
	static const int BaseHeight = 80;

	Pixel(SpriteBatch *spriteBatch,
		  SpriteAnim *spriteAnim,
		  const TexPart &pixelFrame,
		  int x,
		  int y);
	
	~Pixel();
	
	void StartCorrectColorAnim(const Color &color);
	void StartWrongColorAnim(const Color &color);

	void Update(float time, float ms) {};
	void Update(float time, float ms, float boardScale, const sm::Vec2 &frameShift);
	void Draw(float time, float ms);
	bool IsFinished() const;
	const Color& GetColor() const;
	
	int GetX() const;
	int GetY() const;
};
