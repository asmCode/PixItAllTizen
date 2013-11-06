#pragma once

#include <assert.h>
#include <Utils/GenericSingleton.h>
#include "IPropertyObserver.h"
#include "MessageBox.h"

class MessageBoxManager : public GenericSingleton<MessageBoxManager>
{
private:
	MessageBox *msgBox;
	
public:
	MessageBoxManager();
	
	//! Show mesage box. After close, IPropertyObserver is notified with "msg_closed" command
	void Show(const std::string &msg,
			  IPropertyObserver *propertyObserver,
			  MessageBox::Buttons buttons = MessageBox::Buttons_OkCancel);
	
	/** Show mesage box. After close, IPropertyObserver is notified with "msg_closed" command.
		Additionally you can add some user data to message box control */
	template <typename T>
	void Show(const std::string &msg,
			  IPropertyObserver *propertyObserver,
			  const std::string &userDataName,
			  const T &userData,
			  MessageBox::Buttons buttons = MessageBox::Buttons_OkCancel)
	{
		Show(msg, propertyObserver, buttons);
		
		assert(msgBox != NULL);
		
		msgBox->SetUserData(userDataName, userData);
	}
	
	MessageBox* GetMessageBox() const;
	
	MessageBox::Result GetResult() const;
	
	bool IsMsgBoxVisible() const;
};
