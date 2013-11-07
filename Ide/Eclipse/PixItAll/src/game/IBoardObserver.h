#pragma once

#import <GraphicsLibrary/Color.h>

class IBoardObserver
{
public:
	virtual void BoardInitialized() = 0;
	virtual void PickedWrongColor(int x, int y, const Color &color) = 0;
	virtual void PickedCorrectColor(int x, int y, const Color &color) = 0;
};
