#pragma once

class Image
{
protected:
	int width;
	int height;
	int bpp;	// bits per pixel
	void *data;
	
	Image();
	
public:
	~Image();
	
	static Image* LoadImage(const char *filename);
	
	int GetWidth() const;
	int GetHeight() const;
	int GetBpp() const;
	const void *GetData() const;
};
