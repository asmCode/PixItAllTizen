#include <Graphics/Color.h>
#include <math.h>

void RgbToHsv(short r,
			  short g,
			  short b,
			  short &h,
			  short &s,
			  short &v)
{
	short temp = fmin(fmin(r, g), b);
	
	v = fmax(fmax(r, g), b);
	
	if (temp == v)
		h = 0;
	else
	{
		if (r == v)
			h = 0 + (((g - b) * 60) / (v - temp));
		if (g == v)
			h = 120 + (((b - r) * 60) / (v - temp));
		if (b == v)
			h = 240 + (((r - g) * 60) / (v - temp));
	}
	
	if (h < 0)
		h = h + 360;
	
	if (v == 0)
		s = 0;
	else
		s = ((v - temp) * 100) / v;
	
	v = (short)((100.0 * (float)v) / 255.0f);
}


bool ColorComparer (const Color &color1, const Color &color2)
{
	unsigned int val1 = 0;
	unsigned int val2 = 0;
	
	short h1;
	short s1;
	short v1;
	RgbToHsv(color1.R, color1.G, color1.B, h1, s1, v1);
	
	short h2;
	short s2;
	short v2;
	RgbToHsv(color2.R, color2.G, color2.B, h2, s2, v2);
	
	
//	v1 = (unsigned int)color1.R << 16 | (unsigned int)color1.G << 8 | (unsigned int)color1.B << 0;
//	v2 = (unsigned int)color2.R << 16 | (unsigned int)color2.G << 8 | (unsigned int)color2.B << 0;
	
	val1 = (unsigned int)h1 << 16 | (unsigned int)s1 << 8 | (unsigned int)v1 << 0;
	val2 = (unsigned int)h2 << 16 | (unsigned int)s2 << 8 | (unsigned int)v2 << 0;
	
	return (val1 < val2);
}
