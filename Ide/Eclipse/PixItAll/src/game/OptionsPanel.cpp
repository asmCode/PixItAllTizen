#include "OptionsPanel.h"
#include <GraphicsLibrary/TexPart.h>
#include "ClassContainer.h"
#include "SoundManager.h"
#include "Volume.h"
#include "Fade2.h"
#include "Label.h"
#include "PlayerData.h"
#include "Options.h"
#include "ScreenKeyboard.h"
#include "Leaderboard.h"
#include "LeaderboardControl.h"
#include "XMLElement.h"
#include "Environment.h"
#include "ImagesCollection.h"
#include <GraphicsLibrary/FontRenderer.h>
#include <Utils/Log.h>

OptionsPanel::OptionsPanel()
{
	m_leaderboardControl = NULL;
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
	
	FontRenderer *defaultFont = NULL;
	ClassContainer::GetInstance()->TryGetClass("carter28Font", defaultFont);
	FontRenderer *carter24 = NULL;
	ClassContainer::GetInstance()->TryGetClass("carter24Font", carter24);

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

		//TexPart	tpUserIcon = cc->GetClass<TexPart>("guimap_user_icon");
		TexPart	tpEditButton = cc->GetClass<TexPart>("guimap_edit_btn");
		TexPart	tpEditButtonDown = cc->GetClass<TexPart>("guimap_edit_btn_down");

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
		
		std::string playerName = PlayerData::GetInstance()->m_name;

		int userPosY = 750;
		int userPosX = 30;

		//ret->m_userIcon = new Control(userPosX, userPosY, tpUserIcon);
		//userPosX += 150;
		ret->m_playerNameLabel = new Label(PlayerData::GetInstance()->m_name, defaultFont, Color(234, 120, 60), userPosX, userPosY + 82);
		Label* staticPlayerName = new Label("Player Name", carter24, Color(40, 56, 141), userPosX, userPosY + 28);
		userPosX += defaultFont->MeasureString(playerName.c_str()).X + 30;
		ret->m_editButton = new AnimButton(610, userPosY + 30, tpEditButton, tpEditButtonDown);

		ret->AddChild(ret->btnBack);
		ret->AddChild(ret->soundVol);
		ret->AddChild(ret->musicVol);
		ret->AddChild(ret->imgSoundIcon);
		ret->AddChild(ret->imgMusicIcon);

		//ret->AddChild(ret->m_userIcon);
		ret->AddChild(ret->m_playerNameLabel);
		ret->AddChild(staticPlayerName);
		ret->AddChild(ret->m_editButton);
				
		ObsCast(ITouchObserver, ret ->btnBack) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret ->m_editButton) ->AddObserver(ret);
		ObsCast(IPropertyObserver, ret ->soundVol) ->AddObserver(ret);
		ObsCast(IPropertyObserver, ret ->musicVol) ->AddObserver(ret);
	}
	
	ScreenKeyboard::GetInstance()->SetObserver(ret);

	return ret;
}

void OptionsPanel::TouchPressed(Control *control, int x, int y)
{
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
	
	if (control == btnBack)
	{
		Close();
	}
	else if (control == m_editButton)
	{
		ScreenKeyboard::GetInstance()->SetText(PlayerData::GetInstance()->m_name);
		ScreenKeyboard::GetInstance()->Show();
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

void OptionsPanel::Close()
{
	SetActive(false);

	Options::GetInstance()->SetMusicVolume(sndMng->GetMusicVolume());
	Options::GetInstance()->SetSoundsVolume(sndMng->GetSoundVolume());
	Options::GetInstance()->Save();

	m_leaderboardControl->RefreshCurrentView();
}

void OptionsPanel::ScreenKeyboardDone(const std::string& text)
{
	std::string text2 = text;

	if (text2.size() > 18)
		text2 = text2.substr(0, 18);

	//if (PlayerData::GetInstance()->m_name != text2)
	{
		Log::LogT("Updating player name (%s)", text2.c_str());

		PlayerData::GetInstance()->m_name = text2;
		PlayerData::GetInstance()->Save();


		int playerId = PlayerData::GetInstance()->m_id;
		std::string playerName = PlayerData::GetInstance()->m_name;
		int totalPoints = ImagesCollection::Instance->GetTotalPoints();
		int totalLevels = ImagesCollection::Instance->GetFinishedLevelsCount();

		Leaderboard::GetInstance()->SendPlayerPoints(
			playerId,
			playerName,
			totalPoints,
			totalLevels);

		m_playerNameLabel->SetText(playerName);
		//m_editButton->SetX(m_playerNameLabel->GetTextSize().X + m_playerNameLabel->GetX() + 30);
	}
}

void OptionsPanel::ScreenKeyboardCanceled()
{

}

void OptionsPanel::SetLeaderboardControl(LeaderboardControl* leaderboardControl)
{
	m_leaderboardControl = leaderboardControl;
}
