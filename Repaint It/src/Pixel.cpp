#include "Pixel.h"
#include <Graphics/SpriteBatch.h>
#include <Graphics/SpriteAnim.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include <stdlib.h>

const float Pixel::CorrectAnimCooldown = 0.25f;
const float Pixel::ColorFloodDuration = 1.0f;

Pixel::Pixel(SpriteBatch *spriteBatch,
			 SpriteAnim *spriteAnim,
			 const TexPart &pixelFrame,
			 int x,
			 int y) : baseX(x), baseY(y)
{
	this ->spriteBatch = spriteBatch;
	this ->spriteAnim = spriteAnim;
	this ->pixelFrame = pixelFrame;
	
	this ->colorFloodTime = 0.0f;
	this ->correctAnimProgress = 0.0f;
	
	finished = false;
}

Pixel::~Pixel()
{
}

void Pixel::StartCorrectColorAnim(const Color &color)
{
	mirrorAndFlip = sm::Vec2(rand() % 2 == 0 ? -1.0f : 1.0f, rand() % 2 == 0 ? -1.0f : 1.0f);
	correctAnimProgress = CorrectAnimCooldown;
	colorFloodTime = 0.0f;
	colorFloodValue = 0.0f;
	this ->color = color;
}

void Pixel::StartWrongColorAnim(const Color &color)
{
}

void Pixel::Update(float time, float ms, float boardScale, const sm::Vec2 &frameShift)
{
	correctAnimProgress -= ms;
	correctAnimProgress = fmaxf(correctAnimProgress, 0);
	float angle = (1 - (correctAnimProgress / CorrectAnimCooldown)) * 3.1415f;
	float scaleValue = 1.0f - sinf(angle) * 0.3f;
	
	colorFloodTime += ms;
	colorFloodTime = fminf(colorFloodTime, ColorFloodDuration);
	
	colorTrans =
		sm::Matrix::ScaleMatrix(1.0f / boardScale, 1.0f / boardScale, 1.0f) *
		sm::Matrix::TranslateMatrix((baseX - frameShift.x) * BaseWidth, (baseY - frameShift.y) * BaseHeight, 0) *
		sm::Matrix::TranslateMatrix((float)BaseWidth / 2, (float)BaseHeight / 2, 0) *
		sm::Matrix::ScaleMatrix((float)BaseWidth, (float)BaseHeight, 1.0f);
	
	frameTrans =
		colorTrans *
		sm::Matrix::ScaleMatrix(scaleValue, scaleValue, 1.0f);
	
	if (correctAnimProgress == 0 && colorFloodTime == ColorFloodDuration)
		finished = true;
}

void Pixel::Draw(float time, float ms)
{	
	pixelFrame.Tex->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	const TexPart& frameTexPart = spriteAnim ->GetTexPartAtTime(colorFloodTime);
			
//	spriteBatch ->Draw(Color::White, colorTrans);
	spriteBatch ->Draw(frameTexPart, color, frameTrans * sm::Matrix::ScaleMatrix(mirrorAndFlip.x, mirrorAndFlip.y, 1.0f));
	spriteBatch ->Draw(pixelFrame, Color::White, frameTrans);
}

bool Pixel::IsFinished() const
{
	return finished;
}

const Color& Pixel::GetColor() const
{
	return color;
}

int Pixel::GetX() const
{
	return baseX;
}

int Pixel::GetY() const
{
	return baseY;
}