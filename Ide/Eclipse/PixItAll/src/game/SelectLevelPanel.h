#pragma once

#include "ITouchObserver.h"
#include "Control.h"
#include "Label.h"
#include "AnimButton.h"
#include <list>

class SelectLevelGameState;
class ImagesCollection;
class ImageScrollElement;
class Label;

class SelectLevelPanel :
	public Control,
	public ITouchObserver
{
private:
	static const int PreviewMargin = 40;
	
	unsigned resWidth;
	unsigned resHeight;
	
	AnimButton *btnBack;
	Label *lblTitle;
	Label *lblRes;
	Label *lblPoints;
	
	std::list<ImageScrollElement*> imagesScrollElements;
	ImageScrollElement *actImg;
	
	TexPart imgBg;
	TexPart lvlInfoBg;
	sm::Vec2 previewPos;
	bool centerPreview;
	
	ImagesCollection *imgColl;
	
	unsigned lvlInfoYShift;
	
	SelectLevelGameState *selLvlGameState;
	
	SelectLevelPanel();
	
	void TouchPressed(Control *control, int x, int y);	
	void OnDraw(float time, float ms);
	void OnUpdate(float time, float ms);
	
	std::list<ImageScrollElement*>::iterator GetElementAfterScroll();
	void RefreshImageInfo();
	std::list<ImageScrollElement*>::iterator GetFirstUnlockedUnresolvedLevel();
	
public:
	static SelectLevelPanel *Create(SelectLevelGameState *selLvlGameState,
									ImagesCollection *imgColl);
	
	void HandlePanGesture(IGestureHandler::GestureStatus status,
						  const sm::Vec2 &pos,
						  const sm::Vec2 &trans,
						  const sm::Vec2 &velocity);
	
	void SetActiveImage(std::list<ImageScrollElement*>::iterator it);
	void ScrollToFirstUnlockedUnresolvedLevel();
};
