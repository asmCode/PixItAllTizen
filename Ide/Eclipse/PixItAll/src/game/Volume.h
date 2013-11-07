#pragma once

#include "ITouchObserver.h"
#include "AnimButton.h"
#include "ObserverSubject.h"
#include "IPropertyObserver.h"

class Volume :
	public Control,
	public ITouchObserver,
	public ObserversContainer<IPropertyObserver*>
{
private:
	AnimButton *btnUp;
	AnimButton *btnDown;
	
	unsigned level;
	
	Control *imgLevel[5];
	
	Volume();
	
	void TouchPressed(Control *control, int x, int y);
	
public:
	static Volume *Create();
	void SetLevel(unsigned level);
	unsigned GetLevel() const;
};
