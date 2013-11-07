#pragma once

#include "BaseGameState.h"
#include "IPropertyObserver.h"
#include "IGameCenterViewProxy.h"

class SpriteBatch;
class SelectLevelPanel;
class ImagesCollection;
class ImageInfo;

class SelectLevelGameState : public BaseGameState, public virtual IPropertyObserver
{
private:
	static const std::string RateUrl;
	
	SpriteBatch *sb;
	SelectLevelPanel *selLvlPanel;
	ImagesCollection *imgColl;
	IGameCenterViewProxy *viewProxy;
	
	void PropertyChanged(const std::string &propName,
						 void *sender);
	
public:
	SelectLevelGameState(Game *game, ImagesCollection *imgColl, IGameCenterViewProxy *viewProxy);
	SelectLevelGameState();
	
	Control *GetMainPanel();
	
	void Update(float time, float seconds);
	void Draw(float time, float seconds);
	
	void HandleTapGesture(const sm::Point<int> &point);
	void HandlePress(const sm::Point<int> &point);
	void HandleRelease(const sm::Point<int> &point);	

	void HandlePanGesture(IGestureHandler::GestureStatus status,
						  const sm::Vec2 &pos,
						  const sm::Vec2 &trans,
						  const sm::Vec2 &velocity);
	
	bool Initialize();
	void Release();
	
	void LevelSelected(ImageInfo *imgInfo);
	void BackToMainMenu();
	void ScrollToFirstUnlockedUnresolvedLevel();
	
	void SetFocus();
};
