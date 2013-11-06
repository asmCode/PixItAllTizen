#include "SummaryScreen.h"
#include "ClassContainer.h"
#include <GraphicsLibrary/TexPart.h>
#include "PlayGameState.h"
#include "ImagesCollection.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include "Environment.h"
#include "Label.h"
#include "ImageScrollElement.h"
#include <sstream>
#include <iomanip>
#include <Utils/MemUtils.h>
#include "ColorCounter.h"
#include "RefImage.h"
#include "Fade2.h"
#include "XMLElement.h"
#include "SoundManager.h"

SummaryScreen::SummaryScreen()
{
	bgImgOpacity = 0.0f;
	isActive = false;
	points = 0;
	isRecord = false;
	success = true; // success by default... thats the spirit!
	newRecordProgress = 0.0f;
	
	isWaitingForFade = false;
	actionAfterFade = ActionAfterFade_None;
	
	lblTime = NULL;
	lblCredits = NULL;
	lblFails = NULL;
	lblPoints = NULL;
	imgNewRecord = NULL;
	statsControl = NULL;
	btnPlayAgain = NULL;
	btnNextLevel = NULL;
	btnBackToMenu = NULL;
	btnPlayAgainFail = NULL;
	btnBackToMenuFail = NULL;
}

SummaryScreen::~SummaryScreen()
{
	MemUtils::DeleteObject(lblTime);
	MemUtils::DeleteObject(lblCredits);
	MemUtils::DeleteObject(lblFails);
	MemUtils::DeleteObject(lblPoints);
	MemUtils::DeleteObject(imgNewRecord);
	MemUtils::DeleteObject(statsControl);
	MemUtils::DeleteObject(btnPlayAgain);
	MemUtils::DeleteObject(btnNextLevel);
	MemUtils::DeleteObject(btnBackToMenu);
	MemUtils::DeleteObject(btnPlayAgainFail);
	MemUtils::DeleteObject(btnBackToMenuFail);
	MemUtils::DeleteObject(failPosYAnim);
	MemUtils::DeleteObject(failRotAnim);
}

SummaryScreen *SummaryScreen::Create(ImageInfo *imgInfo,
									 PlayGameState *playGameState)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	FontRenderer *defaultFont = NULL;
	ClassContainer::GetInstance()->TryGetClass("defaultFont", defaultFont);
	
	SummaryScreen *ret = new SummaryScreen();
	if (ret != NULL)
	{
		ret->resWidth = Environment::GetInstance()->GetScreenWidth();
		ret->resHeight = Environment::GetInstance()->GetScreenHeight();
		
		ret->imgInfo = imgInfo;
		ret->playGameState = playGameState;
		
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = ret->resWidth;
		ret ->height = ret->resHeight;
		
		ret->failPosYAnim = InterpolatorFactory::CreateInterpolator<float>(InterpolatorFactory::InterpolationMethod_TCB);
		ret->failPosYAnim->AddKeyframe(0.0f / 3.0f, 0.0f, false);
		ret->failPosYAnim->AddKeyframe(0.15f / 3.0f, 0.315f, false);
		ret->failPosYAnim->AddKeyframe(0.28f / 3.0f, 0.643f, false);
		ret->failPosYAnim->AddKeyframe(0.63f / 3.0f, 0.841f, false);
		ret->failPosYAnim->AddKeyframe(1.03f / 3.0f, 0.903f, false);
		ret->failPosYAnim->AddKeyframe(1.24f / 3.0f, 0.919f, false);
		
		ret->failRotAnim = InterpolatorFactory::CreateInterpolator<float>(InterpolatorFactory::InterpolationMethod_TCB);
		ret->failRotAnim->AddKeyframe(0.0f / 3.0f, 0.0f, false);
		ret->failRotAnim->AddKeyframe(0.15f / 3.0f, 0.315f, false);
		ret->failRotAnim->AddKeyframe(0.28f / 3.0f, 0.643f, false);
		ret->failRotAnim->AddKeyframe(0.63f / 3.0f, 0.841f, false);
		ret->failRotAnim->AddKeyframe(1.13f / 3.0f, 0.903f, false);
		ret->failRotAnim->AddKeyframe(1.44f / 3.0f, 0.919f, false);
		
		TexPart btnPlayAgainTexPart = cc->GetClass<TexPart>("guimap_play_again_btn");
		TexPart btnPlayAgainDownTexPart = cc->GetClass<TexPart>("guimap_play_again_down_btn");
		TexPart btnNextLevelTexPart = cc->GetClass<TexPart>("guimap_next_level_btn");
		TexPart btnNextLevelDownTexPart = cc->GetClass<TexPart>("guimap_next_level_down_btn");
		TexPart btnBackToMenuTexPart = cc->GetClass<TexPart>("guimap_back_to_menu_btn");
		TexPart btnBackToMenuDownTexPart = cc->GetClass<TexPart>("guimap_back_to_menu_down_btn");
		TexPart btnPlayAgainFailTexPart = cc->GetClass<TexPart>("guimap_play_again_fail_btn");
		TexPart btnPlayAgainFailDownTexPart = cc->GetClass<TexPart>("guimap_play_again_fail_down_btn");
		TexPart btnBackToMenuFailTexPart = cc->GetClass<TexPart>("guimap_back_to_menu_fail_btn");
		TexPart btnBackToMenuFailDownTexPart = cc->GetClass<TexPart>("guimap_back_to_menu_fail_down_btn");
		TexPart statsTexPart = cc->GetClass<TexPart>("guimap_summary_stats_bg");
		TexPart tpNewRecord = cc->GetClass<TexPart>("guimap_new_record");
		ret->tpFailed = cc->GetClass<TexPart>("guimap_failed;(");
		
		XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_SummaryScreen");
		XMLElement *guidefBG1 = guidefPanel->GetSubElement(0);
		XMLElement *guidefBG2 = guidefPanel->GetSubElement(1);
		XMLElement *guidefFail = guidefPanel->GetSubElement(2);
		XMLElement *guidefTime = guidefPanel->GetSubElement(3);
		XMLElement *guidefCredits = guidefPanel->GetSubElement(4);
		XMLElement *guidefFails = guidefPanel->GetSubElement(5);
		XMLElement *guidefPoints = guidefPanel->GetSubElement(6);
		XMLElement *guidefRecord = guidefPanel->GetSubElement(7);
		XMLElement *guidefStats = guidefPanel->GetSubElement(8);
		
		ret->bg1X = guidefBG1->GetAttributeInt("posxdiff");
		ret->bg1Y = guidefBG1->GetAttributeInt("posydiff");
		ret->bg2X = guidefBG2->GetAttributeInt("posxdiff");
		ret->bg2Y = guidefBG2->GetAttributeInt("posydiff");
		ret->failBottomY = guidefFail->GetAttributeInt("posybottom");
		
		FindMostFrequentColors(imgInfo, ret->color1, ret->color2);
		
		ret->bg1 = cc->GetClass<TexPart>("guimap_summary_bg_1");
		ret->bg2 = cc->GetClass<TexPart>("guimap_summary_bg_2");
		
		ret->lblTime = new Label("Time",
								 defaultFont,
								 Color(43, 56, 137),
								 guidefTime->GetAttributeInt("posx"),
								 guidefTime->GetAttributeInt("posy"));
		ret->lblCredits = new Label("Credits",
									defaultFont,
									Color(43, 56, 137),
									guidefCredits->GetAttributeInt("posx"),
									guidefCredits->GetAttributeInt("posy"));
		ret->lblFails = new Label("Fails",
								  defaultFont,
								  Color(43, 56, 137),
								  guidefFails->GetAttributeInt("posx"),
								  guidefFails->GetAttributeInt("posy"));
		ret->lblPoints = new Label("Points",
								   defaultFont,
								   Color(43, 56, 137),
								   guidefPoints->GetAttributeInt("posx"),
								   guidefPoints->GetAttributeInt("posy"));
		
		ret->imgNewRecord = new Control(guidefRecord->GetAttributeInt("posx"),
										guidefRecord->GetAttributeInt("posy"),
										tpNewRecord);
		
		ret->statsControl = new Control((ret->resWidth - statsTexPart.ImageRect.Width) / 2 + guidefStats->GetAttributeInt("posxdiff"),
										guidefStats->GetAttributeInt("posy"),
										statsTexPart);
		
		ret->statsControl ->AddChild(ret ->lblTime);
		ret->statsControl ->AddChild(ret ->lblCredits);
		ret->statsControl ->AddChild(ret ->lblFails);
		ret->statsControl ->AddChild(ret ->lblPoints);
		ret->statsControl ->AddChild(ret ->imgNewRecord);
		
		ret->btnPlayAgain = new AnimButton(10, ret->resHeight - btnPlayAgainTexPart.ImageRect.Height - 6,
										   btnPlayAgainTexPart,
										   btnPlayAgainDownTexPart);
		ret->btnBackToMenu = new AnimButton((ret->resWidth - btnBackToMenuTexPart.ImageRect.Width) / 2,
											ret->resHeight - btnBackToMenuTexPart.ImageRect.Height - 6,
											btnBackToMenuTexPart,
											btnBackToMenuDownTexPart);
		ret->btnPlayAgainFail = new AnimButton(10, ret->resHeight - btnPlayAgainFailTexPart.ImageRect.Height - 6,
											   btnPlayAgainFailTexPart,
											   btnPlayAgainFailDownTexPart);
		ret->btnBackToMenuFail = new AnimButton((ret->resWidth - btnBackToMenuFailTexPart.ImageRect.Width) / 2,
												ret->resHeight - btnBackToMenuFailTexPart.ImageRect.Height - 6,
												btnBackToMenuFailTexPart,
												btnBackToMenuFailDownTexPart);
		ret->btnNextLevel = new AnimButton(ret->resWidth - btnNextLevelTexPart.ImageRect.Width - 6,
										   ret->resHeight - btnNextLevelTexPart.ImageRect.Height - 6,
										   btnNextLevelTexPart,
										   btnNextLevelDownTexPart);
		
		ret->AddChild(ret->btnPlayAgain);
		ret->AddChild(ret->btnNextLevel);
		ret->AddChild(ret->btnBackToMenu);
		ret->AddChild(ret->btnPlayAgainFail);
		ret->AddChild(ret->btnBackToMenuFail);

		ret->AddChild(ret->statsControl);
				
		ObsCast(ITouchObserver, ret->btnPlayAgain) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->btnNextLevel) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->btnBackToMenu) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->btnPlayAgainFail) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret->btnBackToMenuFail) ->AddObserver(ret);
	}
	
	return ret;
}

void SummaryScreen::FindMostFrequentColors(ImageInfo *imgInfo, Color &color1, Color &color2)
{
	std::vector<ColorCounter::ColorItem> colorItems;
	ColorCounter::CountColors(colorItems, imgInfo->GetRefImage());
	
	if (colorItems.size() >= 2)
	{
		color1 = colorItems[0].color;
		color2 = colorItems[1].color;
	}
	else if (colorItems.size() == 1)
	{
		color1 = colorItems[0].color;
		color2 = colorItems[0].color;	
	}
	else if (colorItems.size() == 0)
	{
		color1 = Color::Black;
		color2 = Color::Black;
	}
}

void SummaryScreen::TouchPressed(Control *control, int x, int y)
{	
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
	
	if (control == btnBackToMenu || control == btnBackToMenuFail)
		playGameState->BackToMenu();
	else if (control == btnPlayAgain || control == btnPlayAgainFail)
	{
		playGameState->PlayAgain();
	}
	else if (control == btnNextLevel)
	{
		isWaitingForFade = true;
		actionAfterFade = ActionAfterFade_NextLevel;
		Fade2::GetInstance()->StartFadeIn(0.4f);
	}
}

void SummaryScreen::SetSuccess(float time,
							   float credits,
							   unsigned fails,
							   unsigned points,
							   bool isRecord)
{
	this->points = points;
	this->guiPoints = 0;
	this->isRecord = isRecord;
	success = true;
	newRecordProgress = 0.0f;
	imgNewRecord->SetVisible(false);
	
	std::stringstream txt;
	
	txt << std::setfill('0') << std::setw(2) << (unsigned)time / 60 << ":" << std::setw(2) << (unsigned)time % 60;
	lblTime->SetText(txt.str());
	
	txt.str("");
	txt << (int)credits;
	lblCredits->SetText(txt.str());
	
	txt.str("");
	txt << fails;
	lblFails->SetText(txt.str());
	
	txt.str("");
	txt << points;
	lblPoints->SetText(txt.str());
	
	lblTime->SetVisible(false);
	lblCredits->SetVisible(false);
	lblFails->SetVisible(false);
	lblPoints->SetVisible(false);
	statsControl->SetVisible(true);
	
	btnNextLevel->SetVisible(true);
	btnPlayAgain->SetVisible(true);
	btnBackToMenu->SetVisible(true);
	
	btnPlayAgainFail->SetVisible(false);
	btnBackToMenuFail->SetVisible(false);
}

void SummaryScreen::SetFailure()
{
	success = false;
	failProgress = 0.0f;
	
	failedTrans = sm::Matrix::ScaleMatrix(0.0f, 0.0f, 0.0f);

	imgNewRecord->SetVisible(false);
	lblTime->SetVisible(false);
	lblCredits->SetVisible(false);
	lblFails->SetVisible(false);
	lblPoints->SetVisible(false);
	statsControl->SetVisible(false);
	
	btnNextLevel->SetVisible(false);
	btnPlayAgain->SetVisible(false);
	btnBackToMenu->SetVisible(false);
	
	btnPlayAgainFail->SetVisible(true);
	btnBackToMenuFail->SetVisible(true);
}

void SummaryScreen::OnUpdate(float time, float seconds)
{
	if (isWaitingForFade && Fade2::GetInstance()->IsReady())
	{
		isWaitingForFade = false;
		Fade2::GetInstance()->StartFadeOut(0.4f);
		
		switch (actionAfterFade)
		{
			case ActionAfterFade_None:
				break;
				
			case ActionAfterFade_NextLevel:
				playGameState->NextLevel();
				break;
				
			case ActionAfterFade_PlayAgain:
				//playGameState->PlayAgain();
				break;
		}
	}
	
	bgImgOpacity = fminf(bgImgOpacity += seconds * 1.0f, 1.0f);
	statsControl->SetOpacity(bgImgOpacity);
	
	summaryProgress += seconds;
	
	if (success)
	{
		if (summaryProgress >= 1.0f && !lblTime->IsVisible())
		{
			lblTime->SetVisible(true);
			SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Coin);
		}
		if (summaryProgress >= 1.4f && !lblCredits->IsVisible())
		{
			lblCredits->SetVisible(true);
			SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Coin);
		}
		if (summaryProgress >= 1.8f && !lblFails->IsVisible())
		{
			lblFails->SetVisible(true);
			SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Coin);
		}
		if (summaryProgress >= 2.2f && !lblPoints->IsVisible())
		{
			lblPoints->SetVisible(true);
			SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Coin);
		}
		
		if (summaryProgress < 2.2f)
			guiPoints = 0;
		else if (summaryProgress >= 3.5f)
			guiPoints = points;
		else
		{
			guiPoints = (unsigned)((float)points * (summaryProgress - 2.2f) / (3.5 - 2.2f));
		}
		
		if (isRecord && summaryProgress >= 4.0f)
		{
			newRecordProgress += seconds;
			newRecordProgress = fminf(1.0f, newRecordProgress);
			
			imgNewRecord->SetVisible(true);
		}
		
		std::stringstream txt;
		txt << guiPoints;
		lblPoints->SetText(txt.str());
	}
	
	if (!success && summaryProgress >= 1.2f)
	{
		float failPosY;
		float failAngle;
		failPosYAnim->GetValue(failProgress, failPosY);
		failRotAnim->GetValue(failProgress, failAngle);
		
		failedTrans =
			sm::Matrix::TranslateMatrix(resWidth / 2, failPosY * (float)failBottomY, 0) *
			sm::Matrix::RotateAxisMatrix(failAngle * 0.25f, 0, 0, 1) *
			sm::Matrix::ScaleMatrix(tpFailed.ImageRect.Width, tpFailed.ImageRect.Height, 1.0f);
		
		failProgress += seconds;
	}
}

bool SummaryScreen::IsActive() const
{
	return isActive;
}

void SummaryScreen::SetActive(bool active)
{
	isActive = active;
	
	if (active)
	{
		Fade2::GetInstance()->StartFadeOut(0.4f);
		
		bgImgOpacity = 0.0f;
		summaryProgress = 0.0f;
	}
}

void SummaryScreen::OnDraw(float time, float seconds)
{
	sm::Vec2 globalPos = GetGlobalPos();
	spriteBatch ->Draw(Color(255, 255, 255), 0, 0, width, height);
	if (success)
	{
		spriteBatch ->Draw(bg1, color1, (int)globalPos.x + bg1X, (int)globalPos.y - bg1Y);
		spriteBatch ->Draw(bg2, color2, (int)globalPos.x + bg2X, (int)globalPos.y - bg2Y);
	}
	else
	{
		spriteBatch ->Draw(bg1, Color::Black, (int)globalPos.x + bg1X, (int)globalPos.y - bg1Y);
		spriteBatch ->Draw(bg2, Color::Black, (int)globalPos.x + bg2X, (int)globalPos.y - bg2Y);
	}
	
	if (!success)
	{
		spriteBatch->Draw(tpFailed, Color::White, failedTrans);
	}
}
