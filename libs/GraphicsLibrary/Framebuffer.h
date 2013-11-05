#pragma once

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

class Framebuffer
{
private:
	unsigned framebufferId;
	unsigned textureId;
	unsigned depthRenderBufferId;
	unsigned offscreenColorRenderBufferId;

	unsigned prevFramebufferId;

	int width;
	int height;
	int bpp;

public:
	Framebuffer();
	~Framebuffer(void);

	void StartOffscreenRendering();
	void StopOffscreenRendering();

	bool Initialize(int width, int height, int bpp);
	unsigned GetTextureId();

	void BindFramebuffer();
	void Validate();

	void AttachColorTexture(unsigned textureId);
	void AttachDepthTexture(unsigned textureId);

	int GetWidth();
	int GetHeight();
	int GetBpp();

	static void RestoreDefaultFramebuffer();
};
