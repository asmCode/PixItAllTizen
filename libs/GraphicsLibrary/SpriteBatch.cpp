#include "SpriteBatch.h"
#include "../Utils/Math/Vec2.h"
#include "../Utils/Math/Vec3.h"
#include "OpenglPort.h"
#import <memory.h>

const float SpriteBatch::Verts[8] = {
	-0.5f, -0.5f,	
	 0.5f, -0.5f,
	-0.5f,  0.5f,
	 0.5f,  0.5f
};

const float SpriteBatch::Coords[8] = {
	0, 0,	
	1, 0,
	0, 1,
	1, 1
};

const unsigned char SpriteBatch::ColorMask[16] = {
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255
};

SpriteBatch::SpriteBatch()
{
	int viewportBound[4];
	glGetIntegerv(GL_VIEWPORT, viewportBound);
	proj = sm::Matrix::Ortho2DMatrix(0, viewportBound[2], viewportBound[3], 0);
}

void SpriteBatch::Begin()
{
	glGetFloatv(GL_PROJECTION_MATRIX, prevProjMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, prevModelMatrix);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(proj);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	bool isTex = glIsEnabled(GL_TEXTURE_2D);
	bool isDepth = glIsEnabled(GL_DEPTH_TEST);
	bool isBlend = glIsEnabled(GL_BLEND);
	
	glDisable(GL_DEPTH_TEST);
	
	if (isTex) glEnable(GL_TEXTURE_2D);
	if (isDepth) glEnable(GL_DEPTH_TEST);
	if (isBlend) glEnable(GL_BLEND);
}

void SpriteBatch::End()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(prevProjMatrix);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(prevModelMatrix);	
}

void SpriteBatch::Draw(const Color &color, int x, int y, int width, int height)
{
	const float verts[8] = {
		x, y,
		x + width, y,
		x, y + height,
		x + width, y + height
	};
	
	const unsigned char _color[16] = {
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A
	};	
	
	Draw(NULL, verts, NULL, _color);
}

void SpriteBatch::Draw(Texture *tex, int x, int y, int width, int height)
{
	const float verts[8] = {
		x, y,
		x + width, y,
		x, y + height,
		x + width, y + height
	};
	
	Draw(tex, verts, Coords, NULL);	
}

void SpriteBatch::Draw(Texture *tex, int x, int y)
{
	Draw(tex, x, y, tex ->GetWidth(), tex ->GetHeight());
}

void SpriteBatch::Draw(Texture *tex, const float *texCoords, int x, int y, int width, int height)
{
	const float verts[8] = {
		x, y,
		x + width, y,
		x, y + height,
		x + width, y + height
	};
	
	Draw(tex, verts, texCoords, NULL);
}

void SpriteBatch::Draw(const TexPart &texPart, int x, int y)
{
	Draw(texPart, x, y, texPart.ImageRect.Width, texPart.ImageRect.Height);
}

void SpriteBatch::Draw(const TexPart &texPart, int x, int y, int width, int height)
{
	const float verts[8] = {
		x, y,
		x + width, y,
		x, y + height,
		x + width, y + height
	};
	
	Draw(texPart.Tex, verts, texPart.TexCoords, NULL);
}

void SpriteBatch::Draw(const TexPart &texPart, const Color &colorMask, int x, int y)
{
	Draw(texPart, colorMask, x, y, texPart.ImageRect.Width, texPart.ImageRect.Height);
}

void SpriteBatch::Draw(const TexPart &texPart, const Color &colorMask, int x, int y, int width, int height)
{
	const float verts[8] = {
		x, y,
		x + width, y,
		x, y + height,
		x + width, y + height
	};
	
	const unsigned char _color[16] = {
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A
	};
	
	Draw(texPart.Tex, verts, texPart.TexCoords, _color);
}

void SpriteBatch::Draw(const Color &color, sm::Matrix trans)
{
	sm::Vec3 corners[4] = {
		sm::Vec3(-0.5f, -0.5f, 0.0f),
		sm::Vec3( 0.5f, -0.5f, 0.0f),
		sm::Vec3(-0.5f,  0.5f, 0.0f),
		sm::Vec3( 0.5f,  0.5f, 0.0f)
	};
	
	for (unsigned i = 0; i < 4; i++)
		corners[i] = trans * corners[i];
	
	float verts[8] = {
		corners[0].x, corners[0].y,
		corners[1].x, corners[1].y,
		corners[2].x, corners[2].y,
		corners[3].x, corners[3].y
	};
	
	const unsigned char _color[16] = {
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A
	};
	
	Draw(NULL, verts, NULL, _color);
}

void SpriteBatch::Draw(const TexPart &texPart, const Color &colorMask, const sm::Matrix &trans)
{
	sm::Vec3 corners[4] = {
		sm::Vec3(-0.5f, -0.5f, 0.0f),
		sm::Vec3( 0.5f, -0.5f, 0.0f),
		sm::Vec3(-0.5f,  0.5f, 0.0f),
		sm::Vec3( 0.5f,  0.5f, 0.0f)
	};
	
	for (unsigned i = 0; i < 4; i++)
		corners[i] = trans * corners[i];
	
	float verts[8] = {
		corners[0].x, corners[0].y,
		corners[1].x, corners[1].y,
		corners[2].x, corners[2].y,
		corners[3].x, corners[3].y
	};
	
	const unsigned char _color[16] = {
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A
	};
	
	Draw(texPart.Tex, verts, texPart.TexCoords, _color);
}

void SpriteBatch::Draw(Texture *tex,
		  const float *verts,
		  const float *coords,
		  const unsigned char *colorMask)
{
	if (tex != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		tex ->BindTexture();
	}
	else
		glDisable(GL_TEXTURE_2D);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, verts);
	
	if (coords != NULL)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, coords);
	}
	
	if (colorMask != NULL)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorMask);
	}
	
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	if (coords != NULL)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (colorMask != NULL)
		glDisableClientState(GL_COLOR_ARRAY);
}
