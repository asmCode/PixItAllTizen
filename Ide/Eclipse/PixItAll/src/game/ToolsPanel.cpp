#include "ToolsPanel.h"
#include "PalettePanel.h"
#include "Environment.h"
#include "ClassContainer.h"
#include "Player.h"
#include "ClassContainer.h"
#include "ColorButton.h"
#include "LevelController.h"
#include "XMLElement.h"
#include "SoundManager.h"
#include <GraphicsLibrary/TexPart.h>
#include <GraphicsLibrary/SpriteBatch.h>
#include <GraphicsLibrary/FontRenderer.h>
#include <Utils/MemUtils.h>
#include <Utils/Utils/Randomizer.h>
#include <sstream>
#include <iomanip>
#include <stdio.h>

const float ToolsPanel::coinShakeDuration = 0.5f;
const float ToolsPanel::noMoneyDuration = 0.5f;

ToolsPanel::ToolsPanel()
{
	coinShakeVal = 0.0f;
	coinShakeTimeLeft = 0.0f;
	
	noMoneyVal = 0.0f;
	noMoneyTimeLeft = 0.0f;
	
	toolsStripBg = NULL;
	previewBtn = NULL;
	imgCoin = NULL;
	imgX2 = NULL;
	imgNoMoney = NULL;
	pauseBtn = NULL;
	paletteBtn = NULL;
	lblTime = NULL;
	lblCredits = NULL;
}

ToolsPanel::~ToolsPanel()
{
	MemUtils::DeleteObject(toolsStripBg);
	MemUtils::DeleteObject(previewBtn);
	MemUtils::DeleteObject(imgCoin);
	MemUtils::DeleteObject(imgX2);
	MemUtils::DeleteObject(imgNoMoney);
	MemUtils::DeleteObject(pauseBtn);
	MemUtils::DeleteObject(paletteBtn);
	MemUtils::DeleteObject(lblTime);
	MemUtils::DeleteObject(lblCredits);
}

ToolsPanel *ToolsPanel::Create(PalettePanel *palettePanel, Player *player, Color activeColor)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	
	ToolsPanel *ret = new ToolsPanel();
	if (ret != NULL)
	{
		ret->resWidth = PixItAll::Environment::GetInstance()->GetScreenWidth();
		ret->resHeight = PixItAll::Environment::GetInstance()->GetScreenHeight();
		
		ret->defaultFont = NULL;
		ClassContainer::GetInstance()->TryGetClass("defaultFont", ret->defaultFont);
		
		XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_ToolsPanel");
		XMLElement *guidefCoin = guidefPanel->GetSubElement(0);
		XMLElement *guidefX2 = guidefPanel->GetSubElement(1);
		XMLElement *guidefNoMoney = guidefPanel->GetSubElement(2);
		XMLElement *guidefPause = guidefPanel->GetSubElement(3);
		XMLElement *guidefPreview = guidefPanel->GetSubElement(4);
		XMLElement *guidefPalette = guidefPanel->GetSubElement(5);
		XMLElement *guidefLabelCredits = guidefPanel->GetSubElement(6);
		XMLElement *guidefLabelTime = guidefPanel->GetSubElement(7);
		
		ret->coinX = guidefCoin->GetAttributeInt("posx");
		ret->coinY = guidefCoin->GetAttributeInt("posy");
		ret->lblCreditsX = guidefLabelCredits->GetAttributeInt("posx");
		ret->lblTimeX = guidefLabelTime->GetAttributeInt("posx");
		
		ret ->palettePanel = palettePanel;
		ret ->player = player;
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = ret->resWidth;
		ret ->height = ret->resHeight;
	
		ret ->imgCoin = new Control(ret->coinX, ret->coinY, cc->GetClass<TexPart>("guimap_coin"));
		ret ->imgX2 = new Control(guidefX2->GetAttributeInt("posx"),
								  guidefX2->GetAttributeInt("posy"),
								  cc->GetClass<TexPart>("guimap_double_points"));
		ret ->imgNoMoney = new Control(guidefNoMoney->GetAttributeInt("posx"),
									   guidefNoMoney->GetAttributeInt("posy"),
									   cc->GetClass<TexPart>("guimap_no_money"));
		ret ->imgNoMoney->SetOpacity(0.0f);
		ret ->pauseBtn = new AnimButton(guidefPause->GetAttributeInt("posx"),
										guidefPause->GetAttributeInt("posy"),
										cc->GetClass<TexPart>("guimap_pause"),
										cc->GetClass<TexPart>("guimap_pause_down"));
		ret ->previewBtn = new Control(guidefPreview->GetAttributeInt("posx"),
									   guidefPreview->GetAttributeInt("posy"),
									   cc->GetClass<TexPart>("guimap_preview_btn"));
		ret ->paletteBtn = new ColorButton(guidefPalette->GetAttributeInt("posx"),
										   guidefPalette->GetAttributeInt("posy"),
										   cc->GetClass<TexPart>("guimap_palette_btn"),
										   activeColor);
		
		sm::Point<int> creditsLblSize = ret->defaultFont->MeasureString("0");
		sm::Point<int> timeLblSize = ret->defaultFont->MeasureString("00:00");
		
		ret ->lblCredits = new Label("0",
									 ret->defaultFont,
									 Color(43, 56, 137),
									 ret->lblCreditsX - creditsLblSize.X,
									 guidefLabelCredits->GetAttributeInt("posy"));
		ret ->lblTime = new Label("00:00",
								  ret->defaultFont,
								  Color(43, 56, 137),
								  ret->lblTimeX - timeLblSize.X,
								  guidefLabelTime->GetAttributeInt("posy"));
		
		TexPart bg = cc->GetClass<TexPart>("guimap_panel_strip");
		ret ->toolsStripBg = new Control(0,
										 ret->resHeight - bg.ImageRect.Height,
										 bg);
		ret ->toolsStripBg ->AddChild(ret ->previewBtn);
		ret ->toolsStripBg ->AddChild(ret ->pauseBtn);
		ret ->toolsStripBg ->AddChild(ret ->paletteBtn);
		ret ->toolsStripBg ->AddChild(ret ->imgNoMoney);
		ret ->toolsStripBg ->AddChild(ret ->lblTime);
		ret ->toolsStripBg ->AddChild(ret ->lblCredits);
		ret ->toolsStripBg ->AddChild(ret ->imgCoin);
		ret ->toolsStripBg ->AddChild(ret ->imgX2);
		
		ret->imgX2->SetVisible(false);
		
		ret ->AddChild(ret ->toolsStripBg);
		
		ObsCast(ITouchObserver, ret ->paletteBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret ->previewBtn) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret ->pauseBtn) ->AddObserver(ret);
	}
	
	return ret;
}

void ToolsPanel::SetLevelController(LevelController *lvlCtrl)
{
	this->lvlCtrl = lvlCtrl;
}

void ToolsPanel::OnTouch(int x, int y)
{
}

void ToolsPanel::PaletteColorChanged(const Color &color)
{
	paletteBtn ->SetColor(color);
}

void ToolsPanel::TouchPressed(Control *control, int x, int y)
{
	if (control == paletteBtn)
	{
		if (palettePanel ->IsRolled())
			palettePanel ->Unroll();
		else
			palettePanel ->Roll();
	}
	else if (control == pauseBtn)
	{
		SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
		lvlCtrl->ShowInGameMenu();
	}
	else if (control == previewBtn)
		lvlCtrl->EndPreview();
}

void ToolsPanel::TouchBegin(Control *control, int x, int y)
{
	if (control == previewBtn)
		lvlCtrl->StartPreview();
}

void ToolsPanel::TouchEnd(Control *control, int x, int y)
{
	if (control == previewBtn)
		lvlCtrl->EndPreview();
}

bool ToolsPanel::IsVisible()
{
	return true;
}

bool ToolsPanel::IsClosing()
{
	return false;
}

void ToolsPanel::Hide()
{
}

void ToolsPanel::Show()
{
}

void ToolsPanel::PropertyChanged(const std::string &propName, void *sender)
{
	if (player != sender)
		return;
	
	char txt[32];
	
	std::stringstream ss;
	
	if (propName == "timeElapsed")
	{
		float time = player ->GetTime();
		sprintf(txt, "%02u:%02u", (unsigned)time / 60, (unsigned)time % 60);
		sm::Point<int> size = defaultFont->MeasureString(txt);
		lblTime->SetX(lblTimeX - size.X);
		lblTime->SetText(txt);
	}
	else if (propName == "credits")
	{
		int credits = (int)player ->GetCredits();
		ss << std::setw(5) << credits;
		sm::Point<int> size = defaultFont->MeasureString(ss.str().c_str());
		//sprintf(txt, "%05u", credits);
		lblCredits->SetX(lblCreditsX - size.X);
		lblCredits->SetText(ss.str());
		
		if (credits > 0)
			StartShakingCoin();
		else
			NotifyNoMoney();
	}
}

void ToolsPanel::StartShakingCoin()
{
	coinShakeTimeLeft = coinShakeDuration;
}

void ToolsPanel::NotifyNoMoney()
{
	noMoneyTimeLeft = noMoneyDuration;
}

void ToolsPanel::OnUpdate(float time, float ms)
{
	int _coinX = coinX;
	int _coinY = coinY;

	if (coinShakeTimeLeft > 0.0f)
	{		
		coinShakeTimeLeft = fmaxf(0.0f, coinShakeTimeLeft - ms);
		coinShakeVal = coinShakeTimeLeft / coinShakeDuration;
				
		Randomizer random;
			
		_coinX += (int)(float)random.GetInt(-2, 2) * coinShakeVal;
		_coinY += (int)(float)random.GetInt(-2, 2) * coinShakeVal;
		
		imgCoin->SetPosition(_coinX, _coinY);
	}
	else
	{
		imgCoin->SetPosition(_coinX, _coinY);
	}
	
	if (noMoneyTimeLeft > 0.0f)
	{		
		noMoneyTimeLeft = fmaxf(0.0f, noMoneyTimeLeft - ms);
		noMoneyVal = noMoneyTimeLeft / noMoneyDuration;
		
		imgNoMoney->SetOpacity(noMoneyVal * 0.7f);
	}
	else
		imgNoMoney->SetOpacity(0.0f);
}

void ToolsPanel::EnablePaletteButton(bool enable)
{
	paletteBtn->SetEnabled(enable);
}

void ToolsPanel::EnablePreviewButton(bool enable)
{
	previewBtn->SetEnabled(enable);
}

void ToolsPanel::ShowDoubleBonus(bool show)
{
	imgX2->SetVisible(show);
}

void ToolsPanel::SetCredits(int credits)
{
	std::stringstream ss;
	ss << std::setw(5) << credits;
	sm::Point<int> size = defaultFont->MeasureString(ss.str().c_str());
	lblCredits->SetX(lblCreditsX - size.X);
	lblCredits->SetText(ss.str());
}

void ToolsPanel::ShowPauseButton(bool show)
{
	pauseBtn->SetVisible(show);
}
