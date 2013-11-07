#include <iostream>

#include "ColorCounter.h"
#include "Palette.h"
#include <assert.h>
#include <algorithm>

bool ColorItemComparer(ColorCounter::ColorItem c1, ColorCounter::ColorItem c2)
{
	return c1.count < c2.count;
}


ColorCounter::ColorItem::ColorItem(Color color)
{
	count = 0;
	this->color =color;
}

void ColorCounter::CountColors(std::vector<ColorCounter::ColorItem> &colorItems,
							   const IImage *image)
{
	colorItems.clear();
	
	std::vector<Color> colors;
	Palette::GetPaletteColors(image, colors);
	
	for (unsigned i = 0; i < colors.size(); i++)
		colorItems.push_back(ColorItem(colors[i]));
	
	for (unsigned y = 0; y < image->GetHeight(); y++)
	{
		for (unsigned x = 0; x < image->GetWidth(); x++)
		{
			Color color = image->GetColor(x, y);
			ColorItem *colorItem = FindColorItem(colorItems, color);
			assert(colorItem != NULL);
			
			colorItem->count++;
		}
	}
	
	std::sort(colorItems.begin(), colorItems.end(), ColorItemComparer);
	std::reverse(colorItems.begin(), colorItems.end());
}

ColorCounter::ColorItem *ColorCounter::FindColorItem(std::vector<ColorCounter::ColorItem> &colorItems,
													 const Color &color)
{
	for (unsigned i = 0; i < colorItems.size(); i++)
		if (colorItems[i].color == color)
			return &colorItems[i];
	
	return NULL;
}