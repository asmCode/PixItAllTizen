#pragma once

#include "IPaletteObserver.h"
#include "ITouchObserver.h"
#include "IPropertyObserver.h"
#include "Control.h"
#include "AnimButton.h"
#include "Label.h"

class PalettePanel;
class Player;
class ColorButton;
class LevelController;

class ToolsPanel :
	public Control,
	public IPaletteObserver,
	public ITouchObserver,
	public IPropertyObserver
{
private:
	PalettePanel *palettePanel;
	Player *player;
	Control *toolsStripBg;
	Control *previewBtn;
	Control *imgCoin;
	Control *imgX2;
	Control *imgNoMoney;
	AnimButton *pauseBtn;
	ColorButton *paletteBtn;
	Label *lblTime;
	Label *lblCredits;
	LevelController *lvlCtrl;
	
	unsigned resWidth;
	unsigned resHeight;
	
	unsigned coinX;
	unsigned coinY;
	unsigned lblCreditsX;
	unsigned lblTimeX;
	
	FontRenderer *defaultFont;
	
	float	coinShakeVal;					// current shake value
	float	coinShakeTimeLeft;				// current shake time left
	static const float coinShakeDuration;	// how long shake stay
	
	float	noMoneyVal;						// current shake value
	float	noMoneyTimeLeft;				// current shake time left
	static const float noMoneyDuration;	// how long shake stay
	
	ToolsPanel();
	
	void StartShakingCoin();
	
	void PropertyChanged(const std::string &propName, void *sender);
	
	void OnUpdate(float time, float ms);
//	void OnDraw(float time, float ms);
	
public:
	~ToolsPanel();
	
	static ToolsPanel *Create(PalettePanel *palettePanel, Player *player, Color activeColor);
	
	void SetLevelController(LevelController *lvlCtrl);
	
	void PaletteColorChanged(const Color &color);
	void OnTouch(int x, int y);
	
	void TouchPressed(Control *control, int x, int y);
	void TouchBegin(Control *control, int x, int y);
	void TouchEnd(Control *control, int x, int y);
	
	bool IsVisible();
	bool IsClosing();
	
	void Hide();
	void Show();
	
	void EnablePaletteButton(bool enable);
	void EnablePreviewButton(bool enable);
	void ShowPauseButton(bool show);
	
	void NotifyNoMoney();
	
	void ShowDoubleBonus(bool show);
	
	void SetCredits(int credits);
};
