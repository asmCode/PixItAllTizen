#include "Palette.h"
#include "IImage.h"
#include <algorithm>
#include "AchievementsManager.h"

bool ColorComparer (const Color &color1, const Color &color2);

Palette::Palette()
{
}

void Palette::SetFromImage(const IImage *refImage)
{
	GetPaletteColors(refImage, colors);
	activeColorIndex = 0;
}

const std::vector<Color>& Palette::GetColors() const
{
	return colors;
}

unsigned Palette::GetColorsCount() const
{
	return colors.size();
}

void Palette::SetColor(unsigned index)
{
	if (index != activeColorIndex)
		AchievementsManager::GetInstance()->ChangedColor();
	
	this ->activeColorIndex = index;
	
	ObsCast(IPaletteObserver, this) ->NotifyObservers(&IPaletteObserver::PaletteColorChanged, colors[index]);
}

unsigned Palette::GetActiveColorIndex() const
{
	return activeColorIndex;
}

Color Palette::GetActiveColor() const
{
	return colors[activeColorIndex];
}

bool Palette::HasColor(const std::vector<Color> &colors,
					   const Color &color)
{
	for (unsigned i = 0; i < colors.size(); i++)
		if (colors[i] == color)
			return true;
	
	return false;
}

void Palette::GetPaletteColors(const IImage *img,
							   std::vector<Color> &colors)
{	
	colors.clear();
	for (int x = 0; x < img ->GetWidth(); x++)
	{
		for (int y = 0; y < img ->GetHeight(); y++)
		{
			Color refColor = img ->GetColor(x, y);
			
			if (!HasColor(colors, refColor))
				colors.push_back(refColor);
		}
	}
	
	std::sort(colors.begin(), colors.end(), ColorComparer);
}
