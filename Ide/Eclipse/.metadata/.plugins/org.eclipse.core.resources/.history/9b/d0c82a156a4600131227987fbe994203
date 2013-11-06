#pragma once

#include <Graphics/Color.h>
#include "ObserverSubject.h"
#include "IPaletteObserver.h"
#include <vector>

class IImage;
class Palette;

class Palette : public ObserversContainer<IPaletteObserver*>
{
private:
	unsigned activeColorIndex;
	std::vector<Color> colors;
	Palette *palette;
	
public:
	Palette();
	
	void		SetFromImage(const IImage *refImage);
	const		std::vector<Color>& GetColors() const;
	void		SetColor(unsigned index);
	unsigned	GetColorsCount() const;
	unsigned	GetActiveColorIndex() const;
	Color		GetActiveColor() const;
	
	static void GetPaletteColors(const IImage *img,
								 std::vector<Color> &colors);
	static bool HasColor(const std::vector<Color> &colors,
						 const Color &color);
};
