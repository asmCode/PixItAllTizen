#pragma once

#include "ITouchObserver.h"
#include "IPropertyObserver.h"
#include "AnimButton.h"
#include "IScreenKeyboardObserver.h"

class SoundManager;
class Volume;
class Label;
class LeaderboardControl;

class OptionsPanel :
	public Control,
	public ITouchObserver,
	public IPropertyObserver,
	public IScreenKeyboardObserver
{
private:
	Volume *soundVol;
	Volume *musicVol;
    
    bool isWaitingForFade;
	
	AnimButton *btnBack;

	LeaderboardControl* m_leaderboardControl;
		
	Control *imgSoundIcon;
	Control *imgMusicIcon;
	
	Control *m_userIcon;
	Label *m_playerNameLabel;
	AnimButton *m_editButton;

	SoundManager *sndMng;
	
	bool isActive;
	
	OptionsPanel();
	
	void TouchPressed(Control *control, int x, int y);
	void PropertyChanged(const std::string &propName,
						 void *sender);
	
    void OnUpdate(float time, float seconds);

    void ScreenKeyboardDone(const std::string& text);
    void ScreenKeyboardCanceled();

public:
	static OptionsPanel *Create(SoundManager *sndMng);
	
	bool IsActive() const;
	void SetActive(bool active);
	void SetLeaderboardControl(LeaderboardControl* leaderboardControl);

	void Close();
};
