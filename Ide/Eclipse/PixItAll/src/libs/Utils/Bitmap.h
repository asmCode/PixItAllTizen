#ifndef _BITMAP_
#define _BITMAP_
#pragma once

#include <windows.h>

class Bitmap
{
private:
	HBITMAP handle;
	BITMAP info;

public:
	Bitmap(void)
	{
		handle = NULL;
	}

	~Bitmap(void)
	{
		if (handle != NULL)
		{
			DeleteObject(handle);
			delete [] info.bmBits;
		}
	}

	static Bitmap *FromResources(HINSTANCE hinstance, int id)
	{
		Bitmap *bmp = new Bitmap();
		bmp ->handle = LoadBitmap(hinstance, MAKEINTRESOURCE(id));
		GetObject(bmp ->handle, sizeof(BITMAP), &bmp ->info);
		bmp ->info.bmBits = new BYTE[bmp ->info.bmWidthBytes * bmp ->info.bmHeight];
		GetBitmapBits(bmp ->handle, bmp ->info.bmWidthBytes * bmp ->info.bmHeight, bmp ->info.bmBits);

		return bmp;
	}
	static Bitmap *FromFile(const char *filename)
	{
		Bitmap *bmp = new Bitmap();
		bmp ->handle = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetObject(bmp ->handle, sizeof(BITMAP), &bmp ->info);
		bmp ->info.bmBits = new BYTE[bmp ->info.bmWidthBytes * bmp ->info.bmHeight];
		GetBitmapBits(bmp ->handle, bmp ->info.bmWidthBytes * bmp ->info.bmHeight, bmp ->info.bmBits);

		return bmp;
	}

	static Bitmap *FromMemory(int width, int height, int bpp, BYTE *bits)
	{
		Bitmap *bmp = new Bitmap();
		bmp ->handle = CreateBitmap(width, height, 1, bpp, bits);

		GetObject(bmp ->handle, sizeof(BITMAP), &bmp ->info);
		bmp ->info.bmBits = new BYTE[bmp ->info.bmWidthBytes * bmp ->info.bmHeight];
		GetBitmapBits(bmp ->handle, bmp ->info.bmWidthBytes * bmp ->info.bmHeight, bmp ->info.bmBits);

		return bmp;
	}

	HBITMAP GetHandle()
	{
		return handle;
	}

	Bitmap *GetSubBitmap(int x, int y, int width, int height)
	{
		Bitmap *bmp = new Bitmap();

		BYTE *subData = new BYTE[width * height * info.bmBitsPixel / 8];

		int subDataIndex = 0;
		int pixelSize = info.bmBitsPixel / 8;
		int pos = (y * info.bmWidthBytes) + (x * pixelSize);

		for (int i = y; i < y + height; i++)
		{
			memcpy(&subData[subDataIndex], &((BYTE*)info.bmBits)[pos], width * pixelSize);

			pos += info.bmWidthBytes;
			subDataIndex += width * pixelSize;
		}

		bmp ->handle = CreateBitmap(width, height, 1, info.bmBitsPixel, subData);
		GetObject(bmp ->handle, sizeof(BITMAP), &bmp ->info);
		bmp ->info.bmBits = subData;

		return bmp;
	}

	int GetWidth()
	{
		return info.bmWidth;
	}

	int GetHeight()
	{
		return info.bmHeight;
	}

	int GetBpp()
	{
		return info.bmBitsPixel;
	}

	void GetBits(BYTE *bits)
	{
		memcpy(bits, info.bmBits, info.bmWidthBytes * info.bmHeight);
	}
	void SetBits(BYTE *bits)
	{
		memcpy(info.bmBits, bits, info.bmWidthBytes * info.bmHeight);
		SetBitmapBits(handle, info.bmWidthBytes * info.bmHeight, bits);
	}
};

#endif