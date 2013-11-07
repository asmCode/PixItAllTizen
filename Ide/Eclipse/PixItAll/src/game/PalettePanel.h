#pragma once

#include "IPaletteObserver.h"
#include "ITouchObserver.h"
#include "Control.h"
#include <list>
#include "ColorStrip.h"
#include "IPropertyObserver.h"

class Palette;

typedef std::list<ColorStrip*> ColorStripList;

class PalettePanel :
	public Control,
	public IPaletteObserver,
	public virtual ObserversContainer<IPropertyObserver*>
{
private:
	Palette *palette;
	bool visible;
	bool rolled;
	
	unsigned resWidth;
	unsigned resHeight;
	
	ColorStripList colorStrips;
	std::list<ColorStripList::iterator> colorStripRows;
	
	PalettePanel(Palette *palette);
	
	void ClearColorStripStates(bool clearPeek, bool clearHoover);
	ColorStrip *GetColorStripAtPoint(int x, int y);
	
	/** This is some hack. If he hold finger to loong, then TapGesture won't come,
		but will come HandleRelease. In there we wiil simulate TapGesture. isPressed
		tells if this situation occured */
	bool isPressed;
	
	//void OnTouchEnd(int x, int y);
	
public:
	static const std::string PaletteUnrolledProperty;
	static const std::string PaletteRolledProperty;
	
	~PalettePanel();
	
	static PalettePanel *Create(Palette *palette);
	
	void PaletteColorChanged(const Color &color);
	void OnTouch(int x, int y);

	void HandlePanGesture(IGestureHandler::GestureStatus status,
						  const sm::Vec2 &pos,
						  const sm::Vec2 &trans,
						  const sm::Vec2 &velocity);
	void HandlePress(const sm::Point<int> &point);
	void HandleRelease(const sm::Point<int> &point);
	
	bool IsVisible();
	bool IsClosing();
	
	void Hide();
	void Show();
	
	void Unroll();
	void Roll();
	bool IsRolled();
	
	void Draw(float time, float seconds);
	Palette* GetPalette();
};
