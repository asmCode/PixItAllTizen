#pragma once

#include <Graphics/Color.h>
#include "IImage.h"
#include <vector>

/** Helper class to count how many each color occurs in image */
class ColorCounter
{
public:
	/** Color and number of occurence */
	class ColorItem
	{
	public:
		ColorItem(Color color);
		
		Color color;
		int count;
	};
	
private:
	static ColorItem *FindColorItem(std::vector<ColorCounter::ColorItem> &colorItems,
									const Color &color);
	
public:
	//! Returns descending sorter array of number of occurence of each color in image
	static void CountColors(std::vector<ColorItem> &colorItems,
							const IImage *image);
};

extern bool ColorItemComparer(ColorCounter::ColorItem c1, ColorCounter::ColorItem c2);