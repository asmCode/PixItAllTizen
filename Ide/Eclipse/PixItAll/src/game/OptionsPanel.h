#pragma once

#include "ITouchObserver.h"
#include "IPropertyObserver.h"
#include "AnimButton.h"

class SoundManager;
class Volume;

class OptionsPanel :
	public Control,
	public ITouchObserver,
	public IPropertyObserver
{
private:
	Volume *soundVol;
	Volume *musicVol;
    
    bool isWaitingForFade;
	
	AnimButton *btnBack;
		
	Control *imgSoundIcon;
	Control *imgMusicIcon;
	
	SoundManager *sndMng;
	
	bool isActive;
	
	OptionsPanel();
	
	void TouchPressed(Control *control, int x, int y);
	void PropertyChanged(const std::string &propName,
						 void *sender);
	
    void OnUpdate(float time, float seconds);

public:
	static OptionsPanel *Create(SoundManager *sndMng);
	
	bool IsActive() const;
	void SetActive(bool active);

	void Close();
};
