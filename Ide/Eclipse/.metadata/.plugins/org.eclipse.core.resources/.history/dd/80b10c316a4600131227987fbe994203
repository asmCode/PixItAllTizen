#pragma once

#include "ITouchObserver.h"
#include "AnimButton.h"
#include "IPropertyObserver.h"

class UserImage;
class ToolsPanel;
class PalettePanel;
class BoardView;

class TutorialPanel :
	public Control,
	public ITouchObserver,
	public virtual IPropertyObserver
{
public:
	enum Lesson
	{
		Lesson_Intro = 0,
		Lesson_FourColors,
		Lesson_Preview,
		Lesson_Credits,
		Lesson_Fail,
		Lesson_FailCounter,
		Lesson_Palette,
		Lesson_ZoomMove,
		Lesson_Points,
		LessonsCount,
	};

private:
	const UserImage *userImage;
	ToolsPanel *toolsPanel;
	PalettePanel *palettePanel;
	BoardView *boardView;
	
	Lesson currentLesson;
	
	Control *lessons[LessonsCount];
	
	float fadeDuration;
	float fadeValue;
	float fadeDirection;

	bool isActive;
	
	void OnTouch(int x, int y);
	void OnDraw(float time, float seconds);
	void OnUpdate(float time, float seconds);
	void TouchPressed(Control *control, int x, int y);	
	void PropertyChanged(const std::string &propName, void *sender);
	
	TutorialPanel(const UserImage *userImage);
	
public:
	~TutorialPanel();
	
	static TutorialPanel *Create(const UserImage *userImage,
								 ToolsPanel *toolsPanel,
								 PalettePanel *palettePanel,
								 BoardView *boardView);
	bool IsActive() const;
	void SetActive(bool active);
	
	Lesson GetLesson() const;
};
