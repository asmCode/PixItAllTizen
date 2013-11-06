#pragma once

#import "Texture.h"
#include "TexPart.h"
#import "../Utils/Math/Matrix.h"
#include "../Utils/Math/Vec3.h"
#include "Color.h"

class SpriteBatch
{
private:
	sm::Matrix proj;
	float prevProjMatrix[16];
	float prevModelMatrix[16];
	
	static const float Verts[8];
	static const float Coords[8];
	static const unsigned char ColorMask[16];
	
public:
	SpriteBatch();

	void Begin();
	void End();
	
	void Draw(const Color &color, int x, int y, int width, int height);
	void Draw(Texture *tex, int x, int y, int width, int height);
	void Draw(Texture *tex, int x, int y);
	void Draw(Texture *tex, const float *texCoords, int x, int y, int width, int height);
	void Draw(const TexPart &texPart, int x, int y);
	void Draw(const TexPart &texPart, int x, int y, int width, int height);
	void Draw(const TexPart &texPart, const Color &colorMask, int x, int y);
	void Draw(const TexPart &texPart, const Color &colorMask, int x, int y, int width, int height);
	
	void Draw(const Color &color, sm::Matrix trans);
	void Draw(const TexPart &texPart, const Color &colorMask, const sm::Matrix &trans);
	
	void Draw(
		Texture *tex,
		const float *verts,
		const float *coords,
		const unsigned char *colorMask);
};
