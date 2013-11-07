#pragma once

#include "BaseImage.h"

class UserImage : public BaseImage
{
private:
	UserImage(int width, int height);
	bool *resolvedColors;
	int resolvedCount;
	
public:
	static UserImage *Create(int width, int height);
	void SetColor(int x, int y, const Color &color);
	bool IsColorResolved(int x, int y) const;
	void Clear(const Color &color);
	
	unsigned GetTotalPixels() const;
	unsigned GetResolvedPixels() const;
};
