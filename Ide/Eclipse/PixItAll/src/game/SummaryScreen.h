#pragma once

#include <GraphicsLibrary/IDrawable.h>
#include <GraphicsLibrary/IUpdateable.h>
#include <GraphicsLibrary/TexPart.h>
#include <GraphicsLibrary/Color.h>
#include <GraphicsLibrary/Interpolators/InterpolatorFactory.h>
#include <Utils/Math/Matrix.h>
#include <Utils/Math/Vec2.h>
#include "Control.h"
#include "AnimButton.h"

class ImageInfo;
class SpriteBatch;
class Label;
class PlayGameState;

class SummaryScreen:
	public Control,
	public ITouchObserver
{
private:
	enum ActionAfterFade
	{
		ActionAfterFade_None,
		ActionAfterFade_NextLevel,
		ActionAfterFade_PlayAgain
	};
	
	bool			isWaitingForFade;
	ActionAfterFade	actionAfterFade;
	
	ImageInfo	*imgInfo;
	TexPart		imgBg;
	bool		success;
	float		summaryProgress;
	
	TexPart		bg1; // left part
	TexPart		bg2; // right part

	float		bgImgOpacity;
	
	TexPart		tpFailed;
	sm::Matrix	failedTrans;
	float		failProgress;
	
	IInterpolator<float> *failPosYAnim;
	IInterpolator<float> *failRotAnim;
	
	Color		color1; // color for bg1 (most frequent from image)
	Color		color2; // color for bg1 (second most frequent from image)

	Label		*lblTime;
	Label		*lblCredits;
	Label		*lblFails;
	Label		*lblPoints;
	
	unsigned	resWidth;
	unsigned	resHeight;
	
	unsigned	bg1X;
	unsigned	bg1Y;
	unsigned	bg2X;
	unsigned	bg2Y;
	unsigned	failBottomY;
	
	Control		*imgNewRecord;
	float		newRecordProgress;
	
	Control		*statsControl;
	
	AnimButton	*btnPlayAgain;
	AnimButton	*btnNextLevel;
	AnimButton	*btnBackToMenu;
	AnimButton	*btnPlayAgainFail;
	AnimButton	*btnBackToMenuFail;
	
	PlayGameState *playGameState;
	bool		isActive;
	unsigned	points;		// total points
	unsigned	guiPoints;	// points actually viewed on screen (counting animation)
	bool		isRecord;	// tells if "New Record" image should appear next to the points
	
	/** Find two most requent colors on image. Just for design purpose **/
	static void FindMostFrequentColors(ImageInfo *imgInfo, Color &color1, Color &color2);
	
	SummaryScreen();
	
	void OnDraw(float time, float seconds);
	void OnUpdate(float time, float seconds);
	
	void TouchPressed(Control *control, int x, int y);

public:
	~SummaryScreen();
	
	static SummaryScreen* Create(ImageInfo *imgInfo,
								 PlayGameState *playGameState);
	
//	void HandleTapGesture(const sm::Point<int> &point);
	
	ImageInfo *GetImageInfo();
	
	bool IsActive() const;
	void SetActive(bool active);
	void SetSuccess(float time,
					float credits,
					unsigned fails,
					unsigned points,
					bool isRecord);
	void SetFailure();
};
