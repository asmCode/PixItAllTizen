#pragma once

#import "../Utils/IDisposable.h"

class Texture : public IDisposable
{
protected:
	unsigned texId;

	int width;
	int height;
	int bpp;

public:
	Texture();
	Texture(int width, int height, int bpp, const void *data);
	~Texture();

	int GetWidth() const;
	int GetHeight() const;
	int GetBpp() const;

	void BindTexture();
	unsigned GetId() const;
	
	void SetTextureData(const void *data);	
	void SetTextureData(int x, int y, int width, int height, int bpp, const void *data);
	
};
