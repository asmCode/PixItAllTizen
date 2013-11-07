#include "OptionsPanel.h"
#include <GraphicsLibrary/TexPart.h>
#include "ClassContainer.h"
#include "SoundManager.h"
#include "Volume.h"
#include "Fade2.h"
#include "Options.h"
#include "XMLElement.h"
#include "Environment.h"

OptionsPanel::OptionsPanel()
{
	btnBack = NULL;
	
	soundVol = NULL;
	musicVol = NULL;
	imgSoundIcon = NULL;
	imgMusicIcon = NULL;
	
	sndMng = NULL;
	
	isActive = false;
    isWaitingForFade = false;
}

OptionsPanel *OptionsPanel::Create(SoundManager *sndMng)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	PixItAll::Environment *env = PixItAll::Environment::GetInstance();
	
	OptionsPanel *ret = new OptionsPanel();
	if (ret != NULL)
	{
		float resWidth = env->GetScreenWidth();
		float resHeight = env->GetScreenHeight();
		
		ret->sndMng = sndMng;
		
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = resWidth;
		ret ->height = resHeight;

		sm::Rect<int> bndBack;
		sm::Rect<int> bndBackDown;
		sm::Rect<int> bndSndIcon;
		sm::Rect<int> bndMscIcon;
		
		TexPart backTexPart	= cc->GetClass<TexPart>("guimap_back_btn");
		TexPart	backDownTexPart = cc->GetClass<TexPart>("guimap_back_down_btn");
		TexPart	sndIconTexPart = cc->GetClass<TexPart>("guimap_opt_sound_icon");
		TexPart	mscIconTexPart = cc->GetClass<TexPart>("guimap_opt_music_icon");
		ret->bg = cc->GetClass<TexPart>("guimap_main_menu_bg");
		
		XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_OptionsPanel");
		XMLElement *guidefSoundIcon = guidefPanel->GetSubElement(0);
		XMLElement *guidefSound = guidefPanel->GetSubElement(1);
		XMLElement *guidefMusicIcon = guidefPanel->GetSubElement(2);
		XMLElement *guidefMusic = guidefPanel->GetSubElement(3);
		
		ret->soundVol = Volume::Create();
		ret->soundVol->SetPosition((resWidth - ret->soundVol->GetWidth()) / 2,
								   guidefSound->GetAttributeInt("posy"));
		ret->soundVol->SetLevel((int)(sndMng->GetSoundVolume() * 5));
		ret->musicVol = Volume::Create();
		ret->musicVol->SetPosition((resWidth - ret->musicVol->GetWidth()) / 2,
								   guidefMusic->GetAttributeInt("posy"));
		ret->musicVol->SetLevel((int)(sndMng->GetMusicVolume() * 5));
		
		ret->imgSoundIcon = new Control((resWidth - sndIconTexPart.ImageRect.Width) / 2,
										guidefSoundIcon->GetAttributeInt("posy"),
										sndIconTexPart);
		ret->imgMusicIcon = new Control((resWidth - mscIconTexPart.ImageRect.Width) / 2,
										guidefMusicIcon->GetAttributeInt("posy"),
										mscIconTexPart);
		
		ret->btnBack = new AnimButton(10, resHeight - 10 - backTexPart.ImageRect.Height, backTexPart, backDownTexPart);
		
		ret->AddChild(ret->btnBack);
		ret->AddChild(ret->soundVol);
		ret->AddChild(ret->musicVol);
		ret->AddChild(ret->imgSoundIcon);
		ret->AddChild(ret->imgMusicIcon);
				
		ObsCast(ITouchObserver, ret ->btnBack) ->AddObserver(ret);
		ObsCast(IPropertyObserver, ret ->soundVol) ->AddObserver(ret);
		ObsCast(IPropertyObserver, ret ->musicVol) ->AddObserver(ret);
	}
	
	return ret;
}

void OptionsPanel::TouchPressed(Control *control, int x, int y)
{
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
	
	if (control == btnBack)
	{
		SetActive(false);
		
		Options::GetInstance()->SetMusicVolume(sndMng->GetMusicVolume());
		Options::GetInstance()->SetSoundsVolume(sndMng->GetSoundVolume());
		Options::GetInstance()->Save();
	}
}

void OptionsPanel::PropertyChanged(const std::string &propName,
								   void *sender)
{
	if ((Control*)sender == soundVol)
	{
		SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
		sndMng->SetSoundVolume((float)soundVol->GetLevel() / 5.0f);
	}
	else if ((Control*)sender == musicVol)
	{
		SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
		sndMng->SetMusicVolume((float)musicVol->GetLevel() / 5.0f);
	}
}

void OptionsPanel::OnUpdate(float time, float seconds)
{
	if (isWaitingForFade && Fade2::GetInstance()->IsReady())
	{
		isWaitingForFade = false;
		isActive = false;
		Fade2::GetInstance()->StartFadeOut(0.4f);
	}
}

bool OptionsPanel::IsActive() const
{
	return isActive;
}

void OptionsPanel::SetActive(bool active)
{
    if (active)
	{
        Fade2::GetInstance()->StartFadeOut(0.4f);
		isActive = active;
	}
	else
	{
		isWaitingForFade = true;
		Fade2::GetInstance()->StartFadeIn(0.4f);
	}
}
