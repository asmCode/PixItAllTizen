#include "TutorialPanel.h"
#include "ClassContainer.h"
#include "Environment.h"
#include <GraphicsLibrary/TexPart.h>
#include "PlayGameState.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include "UserImage.h"
#include "ToolsPanel.h"
#include "PalettePanel.h"
#include "BoardView.h"
#include <Utils/Log.h>
#include <Utils/MemUtils.h>
#include "XMLElement.h"

TutorialPanel::TutorialPanel(const UserImage *userImage) : userImage(userImage)
{
	toolsPanel = NULL;
	isActive = true;
	
	fadeDuration = 0.5f;
	fadeDirection = 0.0f;
	fadeValue = 1.0f;
	
	currentLesson = Lesson_Intro;
	
	for (unsigned i = 0; i < LessonsCount; i++)
		lessons[i] = NULL;
}

TutorialPanel::~TutorialPanel()
{
	for (unsigned i = 0; i < LessonsCount; i++)
		MemUtils::DeleteObject(lessons[i]);
}

TutorialPanel *TutorialPanel::Create(const UserImage *userImage,
									 ToolsPanel *toolsPanel,
									 PalettePanel *palettePanel,
									 BoardView *boardView)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	
	TutorialPanel *ret = new TutorialPanel(userImage);
	if (ret != NULL)
	{
		ret->toolsPanel = toolsPanel;
		ret->palettePanel = palettePanel;
		ret->boardView = boardView;
		
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = PixItAll::Environment::GetInstance()->GetScreenWidth();
		ret ->height = PixItAll::Environment::GetInstance()->GetScreenHeight() - 60;
		
		TexPart tpLessons[LessonsCount];
		tpLessons[Lesson_Intro] = cc->GetClass<TexPart>("guimap_tut_intro");
		tpLessons[Lesson_FourColors] = cc->GetClass<TexPart>("guimap_tut_four_colors");
		tpLessons[Lesson_Preview] = cc->GetClass<TexPart>("guimap_tut_preview");
		tpLessons[Lesson_Credits] = cc->GetClass<TexPart>("guimap_tut_credits");
		tpLessons[Lesson_Fail] = cc->GetClass<TexPart>("guimap_tut_fail");
		tpLessons[Lesson_FailCounter] = cc->GetClass<TexPart>("guimap_tut_fail_counter");
		tpLessons[Lesson_Palette] = cc->GetClass<TexPart>("guimap_tut_palette");
		tpLessons[Lesson_ZoomMove] = cc->GetClass<TexPart>("guimap_tut_zoom_move");
		tpLessons[Lesson_Points] = cc->GetClass<TexPart>("guimap_tut_points");
		
		XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_TutorialPanel");
		XMLElement *gdIntro = guidefPanel->GetSubElement(0);
		XMLElement *gdFourColors = guidefPanel->GetSubElement(1);
		XMLElement *gdPreview = guidefPanel->GetSubElement(2);
		XMLElement *gdCredits = guidefPanel->GetSubElement(3);
		XMLElement *gdFail = guidefPanel->GetSubElement(4);
		XMLElement *gdFailCounter = guidefPanel->GetSubElement(5);
		XMLElement *gdPalette = guidefPanel->GetSubElement(6);
		XMLElement *gdZoomMove = guidefPanel->GetSubElement(7);
		XMLElement *gdPoints = guidefPanel->GetSubElement(8);
		
		sm::Point<int> pos[LessonsCount];
		pos[Lesson_Intro] = sm::Point<int>(gdIntro->GetAttributeInt("posx"), gdIntro->GetAttributeInt("posy"));
		pos[Lesson_FourColors] = sm::Point<int>(gdFourColors->GetAttributeInt("posx"), gdFourColors->GetAttributeInt("posy"));
		pos[Lesson_Preview] = sm::Point<int>(gdPreview->GetAttributeInt("posx"), gdPreview->GetAttributeInt("posy"));
		pos[Lesson_Credits] = sm::Point<int>(gdCredits->GetAttributeInt("posx"), gdCredits->GetAttributeInt("posy"));
		pos[Lesson_Fail] = sm::Point<int>(gdFail->GetAttributeInt("posx"), gdFail->GetAttributeInt("posy"));
		pos[Lesson_FailCounter] = sm::Point<int>(gdFailCounter->GetAttributeInt("posx"), gdFailCounter->GetAttributeInt("posy"));
		pos[Lesson_Palette] = sm::Point<int>(gdPalette->GetAttributeInt("posx"), gdPalette->GetAttributeInt("posy"));
		pos[Lesson_ZoomMove] = sm::Point<int>(gdZoomMove->GetAttributeInt("posx"), gdZoomMove->GetAttributeInt("posy"));
		pos[Lesson_Points] = sm::Point<int>(gdPoints->GetAttributeInt("posx"), gdPoints->GetAttributeInt("posy"));
				
		for (unsigned i = 0; i < LessonsCount; i++)
		{
			ret->lessons[i] = new Control(pos[i].X, pos[i].Y, tpLessons[i]);
			ret->AddChild(ret->lessons[i]);
			ret->lessons[i]->SetVisible(false);
			ret->lessons[i]->SetOpacity(0.0f);
		}
		
		ret->lessons[Lesson_Intro]->SetVisible(true);
	}
	
	return ret;
}

bool TutorialPanel::IsActive() const
{
	return isActive;
}

void TutorialPanel::SetActive(bool active)
{
	isActive = active;
}

void TutorialPanel::TouchPressed(Control *control, int x, int y)
{
}

void TutorialPanel::OnTouch(int x, int y)
{
	if (currentLesson == Lesson_FourColors ||
		currentLesson == Lesson_Preview ||
		currentLesson == Lesson_Palette)
	{
	}
	else
	{
		if (fadeValue >= 0.6f)
			fadeDirection = -1;
	}
}

void TutorialPanel::OnDraw(float time, float seconds)
{
}

void TutorialPanel::OnUpdate(float time, float seconds)
{
	if (currentLesson == LessonsCount)
		return;
	
	if (currentLesson == Lesson_FourColors)
	{
		if (userImage->IsColorResolved(3, 2) &&
			userImage->IsColorResolved(4, 2) &&
			userImage->IsColorResolved(3, 3) &&
			userImage->IsColorResolved(4, 3))
			if (fadeValue == 1.0f)
				fadeDirection = -1;			
	}
	
	fadeValue += (seconds / fadeDuration) * fadeDirection;
	fadeValue = fminf(fmaxf(0.0f, fadeValue), 1.0f);
	
	lessons[currentLesson]->SetOpacity(fadeValue);
	
	if (fadeDirection == -1 && fadeValue == 0)
	{
		if (currentLesson == Lesson_Palette)
			fadeDirection = 0;
		else
			fadeDirection = 1;
		
		lessons[currentLesson]->SetVisible(false);
		currentLesson++;
		
		if (currentLesson == Lesson_Preview)
			toolsPanel->EnablePreviewButton(true);
		
		if (currentLesson == Lesson_Palette)
			toolsPanel->EnablePaletteButton(true);
		
		if (currentLesson == LessonsCount)
		{
			SetVisible(false);
			SetActive(false);
		}
		else
			lessons[currentLesson]->SetVisible(true);
	}
}

TutorialPanel::Lesson TutorialPanel::GetLesson() const
{
	return currentLesson;	
}

void TutorialPanel::PropertyChanged(const std::string &propName, void *sender)
{
	if (sender == palettePanel)
	{
		if (currentLesson == Lesson_Palette && propName == PalettePanel::PaletteUnrolledProperty)
		{
			fadeDirection = -1;
		}
		else if ((currentLesson == Lesson_Palette || currentLesson == Lesson_ZoomMove) &&
				 propName == PalettePanel::PaletteRolledProperty)
		{
			if (currentLesson == Lesson_ZoomMove)
				fadeDirection = 1;
			else if (currentLesson == Lesson_Palette)
			{
				lessons[currentLesson]->SetVisible(false);
				currentLesson++;
				lessons[currentLesson]->SetVisible(true);
				fadeDirection = 1;
				fadeValue = 0.0f;
			}
		}
	}
	
	if (sender == boardView)
	{
		if (currentLesson == Lesson_Preview)
		{
			if (propName == BoardView::PreviewHiddenProperty)
				fadeDirection = -1;
		}
		else if (currentLesson == Lesson_ZoomMove)
		{
			if (propName == BoardView::EndedPanProperty || propName == BoardView::EndedPinchProperty)
				fadeDirection = -1;
		}
	}
}
