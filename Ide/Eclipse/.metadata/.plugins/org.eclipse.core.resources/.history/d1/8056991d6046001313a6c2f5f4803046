#pragma once

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

class DepthTexture
{
private:
	unsigned	id;
	int			width;
	int			height;

public:
	DepthTexture(int width, int height);
	~DepthTexture();

	int GetWidth() const;
	int GetHeight() const;

	void BindTexture();
	unsigned GetId();
	void SetTextureData(const unsigned char *data);
};
