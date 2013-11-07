#include "SelectLevelPanel.h"
#include "ClassContainer.h"
#include <GraphicsLibrary/TexPart.h>
#include "SelectLevelGameState.h"
#include "ImagesCollection.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include "ImageScrollElement.h"
#include <sstream>
#include "LevelStats.h"
#include "XMLElement.h"
#include "Environment.h"
#include "SoundManager.h"

SelectLevelPanel::SelectLevelPanel()
{
	imgColl = NULL;
	centerPreview = false;
	previewPos.Set(0, 0);
	
	lvlInfoYShift = 0;
}

SelectLevelPanel *SelectLevelPanel::Create(SelectLevelGameState *selLvlState,
										   ImagesCollection *imgColl)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	FontRenderer *defaultFont = NULL;
	ClassContainer::GetInstance()->TryGetClass("defaultFont", defaultFont);
	
	SelectLevelPanel *ret = new SelectLevelPanel();
	if (ret != NULL)
	{
		ret->resWidth = PixItAll::Environment::GetInstance()->GetScreenWidth();
		ret->resHeight = PixItAll::Environment::GetInstance()->GetScreenHeight();
		
		ret->selLvlGameState = selLvlState;
		ret->imgColl = imgColl;
		
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = ret->resWidth;
		ret ->height = ret->resHeight;
				
		sm::Rect<int> bgBounds;
		sm::Rect<int> visitBtnBounds;
		
		sm::Rect<int> imgBgBounds;

		ret->bg = cc ->GetClass<TexPart>("guimap_main_menu_bg");
		ret->imgBg = cc ->GetClass<TexPart>("guimap_img_bg");
		
		TexPart backTexPart = cc->GetClass<TexPart>("guimap_back_btn");
		TexPart backDownTexPart = cc->GetClass<TexPart>("guimap_back_down_btn");
		TexPart padlock = cc->GetClass<TexPart>("guimap_padlock");
		TexPart tpResolved = cc->GetClass<TexPart>("guimap_lvl_resolved");
		TexPart tutRibbon = cc->GetClass<TexPart>("guimap_tut_tutorial_ribbon");
		ret->lvlInfoBg = cc->GetClass<TexPart>("guimap_lvl_info_bg");
		
		XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_SelectLevelPanel");
		XMLElement *guidefLevelInfoBg = guidefPanel->GetSubElement(0);
		XMLElement *guidefLabelTitle = guidefPanel->GetSubElement(1);
		XMLElement *guidefLabelPoints = guidefPanel->GetSubElement(2);
		XMLElement *guidefLabelRes = guidefPanel->GetSubElement(3);
		XMLElement *guidefFillRectSize = guidefPanel->GetSubElement(4);
		
		unsigned distanceBetweenImages = guidefPanel->GetAttributeInt("img_dist");
		
		ret->lvlInfoYShift = guidefLevelInfoBg->GetAttributeInt("posy");
		
		ret->btnBack = new AnimButton(10, ret->resHeight - backTexPart.ImageRect.Height - 6,
										backTexPart,
										backDownTexPart);

		ret->lblTitle = new Label("Title",
								  defaultFont,
								  Color(43, 56, 137),
								  100,
								  guidefLabelTitle->GetAttributeInt("posy"));
		ret->lblPoints = new Label("Points",
								   defaultFont,
								   Color(43, 56, 137),
								   100,
								   guidefLabelPoints->GetAttributeInt("posy"));

		ret->lblRes = new Label("Res",
								  defaultFont,
								  Color(43, 56, 137),
								  100,
								  guidefLabelRes->GetAttributeInt("posy"));
		
		ret->AddChild(ret->btnBack);
		ret->AddChild(ret->lblTitle);
		ret->AddChild(ret->lblPoints);
		ret->AddChild(ret->lblRes);
		
		ObsCast(ITouchObserver, ret->btnBack)->AddObserver(ret);
		
		sm::Point<float> fillRectSize(guidefFillRectSize->GetAttributeInt("width"),
									  guidefFillRectSize->GetAttributeInt("height"));
		
		int basePos = 0;
		std::list<ImageInfo*> &imgInfoList = imgColl->GetImages();
		std::list<ImageInfo*>::iterator it;
		for (it = imgInfoList.begin(); it != imgInfoList.end(); it++)
		{
			ImageScrollElement *imgScrollEl = new ImageScrollElement((*it),
																	 sm::Vec2(basePos, 0),
																	 fillRectSize,
																	 spriteBatch,
																	 ret->imgBg,
																	 padlock,
																	 tpResolved,
																	 tutRibbon);
			ret->imagesScrollElements.push_back(imgScrollEl);
			basePos += distanceBetweenImages + imgScrollEl->GetTotalWidth();
			ret->AddChild(imgScrollEl);
			ObsCast(ITouchObserver, imgScrollEl) ->AddObserver(ret);
		}
	}
	
	std::list<ImageScrollElement*>::iterator actImgIt = ret->GetFirstUnlockedUnresolvedLevel();
	ret->actImg = (*actImgIt);
	ret->SetActiveImage(actImgIt);
	ret->RefreshImageInfo();
	
	return ret;
}

void SelectLevelPanel::RefreshImageInfo()
{
	int halfRes = resWidth / 2;
	
	ImageInfo *imgInfo = actImg->GetImageInfo();
	std::stringstream txt;
	txt << imgInfo->GetTitle();
	lblTitle->SetText(txt.str());
	lblTitle->SetPosition(halfRes - lblTitle->GetTextSize().X / 2, lblTitle->GetGlobalPos().y);
	txt.str("");
	txt << "Resolution: " << imgInfo->GetWidth() << " x " << imgInfo->GetHeight() << " (" << imgInfo->GetColorsCount() << " colors)";
	lblRes->SetText(txt.str());
	lblRes->SetPosition(halfRes - lblRes->GetTextSize().X / 2, lblRes->GetGlobalPos().y);
	
	txt.str("");
	int points = imgInfo->GetLevelStats()->GetPoints();
	if (points == 0)
		txt << "Never Played";
	else
		txt << "Points: " << imgInfo->GetLevelStats()->GetPoints();
	lblPoints->SetText(txt.str());
	lblPoints->SetPosition(halfRes - lblPoints->GetTextSize().X / 2, lblPoints->GetGlobalPos().y);
}

void SelectLevelPanel::TouchPressed(Control *control, int x, int y)
{
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
	
	ImageScrollElement *imgScrollEl = NULL;
	
	if (control == btnBack)
	{
		selLvlGameState->BackToMainMenu();
	}
	else if ((imgScrollEl = dynamic_cast<ImageScrollElement*>(control)) != NULL)
	{
		selLvlGameState->LevelSelected(imgScrollEl->GetImageInfo());	
	}
}

void SelectLevelPanel::OnDraw(float time, float seconds)
{
	spriteBatch->Draw(bg, 0, 0);
	spriteBatch->Draw(lvlInfoBg, 0, lvlInfoYShift);
	
	std::list<ImageScrollElement*>::iterator it;
	for (it = imagesScrollElements.begin(); it != imagesScrollElements.end(); it++)
	{
		(*it)->SetShift(previewPos + sm::Vec2((resWidth - actImg->GetTotalWidth()) / 2, 0));
		(*it)->Draw(time, seconds);
	}
}

void SelectLevelPanel::OnUpdate(float time, float seconds)
{
	if (centerPreview)
	{
		// TODO: fix function (should be seconds dependant)
		previewPos.x *= 0.80f;
	}
}

void SelectLevelPanel::HandlePanGesture(IGestureHandler::GestureStatus status,
										const sm::Vec2 &pos,
										const sm::Vec2 &trans,
										const sm::Vec2 &velocity)
{
	switch (status)
	{
		case IGestureHandler::GestureStatus_Began:
			centerPreview = false;
			break;
			
		case IGestureHandler::GestureStatus_Changed:
			previewPos = trans;
			break;
			
		case IGestureHandler::GestureStatus_Ended:
			centerPreview = true;
		{
			std::list<ImageScrollElement*>::iterator it =
				GetElementAfterScroll();
			SetActiveImage(it);
		}
			break;
	}
}

std::list<ImageScrollElement*>::iterator SelectLevelPanel::GetElementAfterScroll()
{
	std::list<ImageScrollElement*>::iterator minIt;
	std::list<ImageScrollElement*>::iterator it;
	float min = 999999.0f;
	for (it = imagesScrollElements.begin(); it != imagesScrollElements.end(); it++)
	{
		float dist = fabs((*it)->GetBasePos() + previewPos.x);
		if (dist < min)
		{
			min = dist;
			minIt = it;
		}
	}
	
	return minIt;
}

void SelectLevelPanel::SetActiveImage(std::list<ImageScrollElement*>::iterator selIt)
{
	actImg = (*selIt);
	
	float selectedImgBasePos = (*selIt)->GetBasePos();
	std::list<ImageScrollElement*>::iterator it;
	for (it = imagesScrollElements.begin(); it != imagesScrollElements.end(); it++)
	{
		(*it)->SetBasePos((*it)->GetBasePos() - selectedImgBasePos);
	}
	
	previewPos.x += selectedImgBasePos;
	
	RefreshImageInfo();
}

std::list<ImageScrollElement*>::iterator SelectLevelPanel::GetFirstUnlockedUnresolvedLevel()
{
	std::list<ImageScrollElement*>::iterator it;
	for (it = imagesScrollElements.begin(); it != imagesScrollElements.end(); it++)
	{
		ImageInfo *imgInfo = (*it)->GetImageInfo();
		if (imgInfo == NULL)
			continue;
		
		LevelStats *lvlStats = imgInfo->GetLevelStats();
		if (lvlStats == NULL)
			continue;
		
//		if (lvlStats->IsUnlocked() && lvlStats->GetPoints() == 0 && !imgInfo->IsTutorial())
		if (lvlStats->IsUnlocked() && lvlStats->GetPoints() == 0)
			return it;
	}
	
	return imagesScrollElements.end();
}

void SelectLevelPanel::ScrollToFirstUnlockedUnresolvedLevel()
{
	std::list<ImageScrollElement*>::iterator actImgIt = GetFirstUnlockedUnresolvedLevel();
	if (actImgIt == imagesScrollElements.end())
		return;
	
	actImg = (*actImgIt);
	SetActiveImage(actImgIt);
	RefreshImageInfo();
	centerPreview = true;
}
