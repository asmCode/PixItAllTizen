#pragma once

#import <GraphicsLibrary/IDrawable.h>
#import <GraphicsLibrary/IUpdateable.h>
#import "IBoardObserver.h"
#include "Control.h"
#include "IGestureHandler.h"
#include <Utils/Math/Matrix.h>
#include <list>
#include <GraphicsLibrary/TexPart.h>
#include "ObserverSubject.h"
#include "IPropertyObserver.h"

class BoardManager;
class Texture;
class SpriteBatch;
class SpriteAnim;
class Pixel;
class Palette;
class PalettePanel;
class ParticleManager;
class Splash;
class ToolsPanel;

class BoardView :
	public IBoardObserver,
	public Control,
	public virtual IGestureHandler,
	public virtual ObserversContainer<IPropertyObserver*>
{
private:
	static const int NoPixelFrameTreshold = 5; // stop drawing pixel frames below this value
	static const int ClickableFrameThreshold = 90; // minimum size of frame allowed to click
	static const unsigned ParticleManagerQueueSize = 3;
	static const float ScroolSpeedRelax; // the more value, the faster boards stops after pan gesture

	static const float FirstPreviewDuration;
	static const float GetReadyAfter;
	static const float GetReadyFade;
	static const float GetReadyStill;
	static const float GoAfter;
	static const float GoFade;
	static const float GoStill;
	
	static const float ShowPreviewDuration; // how long it takes to show fully preview
	static const float HidePreviewDuration; // how long it takes to hide preview
	
	Splash			*splash;
	
	BoardManager	*boardMng;
	Palette			*palette;
	PalettePanel	*palettePanel;
	Texture			*imgTex;
	Texture			*refTex;
	Texture			*pixelFrameTex;
	Texture			*floodTex;
	SpriteAnim		*floodAnim;
	TexPart			bgView;
	
	ParticleManager	*partMngQueue[ParticleManagerQueueSize];
	sm::Vec2		partPositions[ParticleManagerQueueSize];
	unsigned		partMngQueueIndex;
	
	float			previewTime;
	float			previewProgress;
	bool			previewMode;
	bool			firstPreviewMode;
	bool			previevKeepTryingClose;
	bool			tutorialMode;
	
	bool			getReadyShown;
	bool			goShown;
	float			splashesProgress;
	TexPart			tpGetReady;
	TexPart			tpGo;
	float			firstPreviewTime;
	
	float			minScale;
	float			maxScale;
	
	bool			smoothView;
	float			smoothStep;
	sm::Vec2		smoothBaseShift;
	float			smoothBaseScale;
	float			scaleToUpdate;
	sm::Vec2		shiftToUpdate;
	
	float			tutBoardScale;
	sm::Vec2		tutFrameShift;
	
	int rowSize;
	int columnSize;
	float boardScale;
	float lastScale;
	sm::Vec2 velocity;
	sm::Matrix pixelTransform;
	bool needToUpdate;
	std::list<Pixel*> activePixels;
	
	float baseWidth;
	float baseHeight;
	
	bool ignoreTap; // is set after presing the screen when velocity is not 0,0

	sm::Vec2 frameShift;
	sm::Vec2 lastFrameShift;
	
	ToolsPanel *toolsPanel;
	
	bool isBonusMode;
	bool isTimeTicking; // true is game is running (after showing Go! splash)
	
	sm::Vec2 BoardToScreen(int x, int y);
		
	bool LoadContent();
	void UpdateActivePixels(float time, float ms);
	
	void UpdatePreview(float time, float seconds);
	void OnUpdate(float time, float ms);
	void OnDraw(float time, float ms);
	void OnTouch(int x, int y);
	
	bool IsVisible();
	bool IsClosing();
	
	void StartBonusOnPixel(int x, int y);
	
public:
	static const std::string PreviewShownProperty;
	static const std::string PreviewHiddenProperty;
	static const std::string StartedPanProperty;
	static const std::string EndedPanProperty;
	static const std::string StartedPinchProperty;
	static const std::string EndedPinchProperty;
	
	BoardView(BoardManager *boardMng,
			  Palette *palette,
			  PalettePanel *palettePanel,
			  bool tutorialMode,
			  ToolsPanel *toolsPanel);
	~BoardView();
	
	bool Initialize();
	void Deinitialize();
	
	void ZoomOutMax();
	void SetBoardView(float scale, const sm::Vec2 &shift);
	
	//float GetBoardScale();
	
	void BoardInitialized();
	void PickedWrongColor(int x, int y, const Color &color);
	void PickedCorrectColor(int x, int y, const Color &color);
	
	void HandlePinchGesture(GestureStatus status, float scale, float velocity);
	void HandlePanGesture(GestureStatus status,
						  const sm::Vec2 &pos,
						  const sm::Vec2 &trans,
						  const sm::Vec2 &velocity);
	void HandleTapGesture(const sm::Point<int> &point);
	void HandlePress(const sm::Point<int> &point);
	
	void DrawBoard();
	
	void HidePreview();
	void ShowPreview();	
	bool IsPrevievActive();
	bool IsFreePreview();
	bool IsTimeTicking();
	
	//! if set, every resolved pixel will explode with color drops
	void SetBonusPixels(bool bonus);
};
