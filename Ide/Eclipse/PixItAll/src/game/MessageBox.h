#pragma once

#include <string>
#include "Control.h"
#include "Label.h"
#include "ObserverSubject.h"
#include "IPropertyObserver.h"

class AnimButton;

class MessageBox :
	public Control,
	public ITouchObserver,
	public ObserversContainer<IPropertyObserver*>

{
public:
	/** Set of buttons that can appear on message box */
	enum Buttons
	{
		Buttons_Ok,			//!< ok only
		Buttons_OkCancel	//!< cancel and ok
	};
	
	/** Button which was pressed */
	enum Result
	{
		Result_Ok,		//!< ok
		Result_Cancel	//!< cancel
	};
	
private:
	void TouchPressed(Control *control, int x, int y);
	
	AnimButton	*btnOk;
	AnimButton	*btnCancel;
	Control		*imgBg;
	Label		*lblText;
	
	Result result;
	
	void OnDraw(float time, float seconds);

public:
	MessageBox(const std::string &message,	//!< Message to show
			   Buttons buttons,				//!< Combination of buttons
			   Color color = Color::Black	//!< Color of font
			   );
	
	void Show();
	
	Result GetResult() const;
};
