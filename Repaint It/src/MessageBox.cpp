#include "MessageBox.h"
#include <Graphics/TexPart.h>
#include <Graphics/SpriteBatch.h>
#include <Graphics/FontRenderer.h>
#include "ClassContainer.h"
#include "Environment.h"
#include "AnimButton.h"
#include "XMLElement.h"
#include "SoundManager.h"

MessageBox::MessageBox(const std::string &message,
					   Buttons buttons,
					   Color color)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_MessageBox");
	XMLElement *guidefOkButton = guidefPanel->GetSubElement(0);
	XMLElement *guidefCancelButton = guidefPanel->GetSubElement(1);
	XMLElement *guidefLabelMsg = guidefPanel->GetSubElement(2);
	
	unsigned resWidth = Environment::GetInstance()->GetScreenWidth();
	unsigned resHeight= Environment::GetInstance()->GetScreenHeight();
	
	x = 0;
	y = 0;
	width = resWidth;
	height = resHeight;
	
	FontRenderer *defaultFont = NULL;
	ClassContainer::GetInstance()->TryGetClass("defaultFont", defaultFont);
	
	TexPart tpOk = cc->GetClass<TexPart>("guimap_yes");
	TexPart tpOkDonw = cc->GetClass<TexPart>("guimap_yes_down");
	TexPart tpCancel = cc->GetClass<TexPart>("guimap_no");
	TexPart tpCancelDonw = cc->GetClass<TexPart>("guimap_no_down");
	TexPart tpMsgBoxBg = cc->GetClass<TexPart>("guimap_msgbox_bg");
	
	btnOk = NULL;
	btnCancel = NULL;
	
	if (buttons == Buttons_Ok || buttons == Buttons_OkCancel)
	{
		btnOk = new AnimButton(tpMsgBoxBg.ImageRect.Width - tpOk.ImageRect.Width - guidefOkButton->GetAttributeInt("posxdiff"),
							   tpMsgBoxBg.ImageRect.Height - tpOk.ImageRect.Height - guidefOkButton->GetAttributeInt("posydiff"),
							   tpOk,
							   tpOkDonw);
	}
	
	if (buttons == Buttons_OkCancel)
	{
		btnCancel = new AnimButton(guidefCancelButton->GetAttributeInt("posxdiff"),
								   tpMsgBoxBg.ImageRect.Height - tpCancel.ImageRect.Height - guidefCancelButton->GetAttributeInt("posydiff"),
								   tpCancel,
								   tpCancelDonw);
	}
	
	imgBg = new Control((resWidth - tpMsgBoxBg.ImageRect.Width) / 2 + 6, // +4 for shadow shift
						(resHeight - tpMsgBoxBg.ImageRect.Height) / 2,
						tpMsgBoxBg.ImageRect.Width,
						tpMsgBoxBg.ImageRect.Height,
						tpMsgBoxBg);
	
	lblText = new Label(message, defaultFont, color, guidefLabelMsg->GetAttributeInt("posx"), guidefLabelMsg->GetAttributeInt("posy"));

	imgBg->AddChild(lblText);
	
	if (btnOk != NULL)
	{
		imgBg->AddChild(btnOk);
		ObsCast(ITouchObserver, btnOk)->AddObserver(this);
	}
	
	if (btnCancel != NULL)
	{
		imgBg->AddChild(btnCancel);
		ObsCast(ITouchObserver, btnCancel)->AddObserver(this);
	}

	AddChild(imgBg);
}

void MessageBox::TouchPressed(Control *control, int x, int y)
{
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_Button);
	
	if (control == btnOk)
	{
		result = Result_Ok;
		visible = false;
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, "closed", this);
	}
	else if (control == btnCancel)
	{
		result = Result_Cancel;
		visible = false;
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, "closed", this);
	}
}

void MessageBox::OnDraw(float time, float seconds)
{
	spriteBatch->Draw(Color(0, 0, 0, 100), 0, 0, width, height);
}

void MessageBox::Show()
{
	visible = true;
}

MessageBox::Result MessageBox::GetResult() const
{
	return result;
}
