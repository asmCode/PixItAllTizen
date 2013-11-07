#include "MessageBoxManager.h"
#include <assert.h>

MessageBoxManager::MessageBoxManager()
{
	msgBox = NULL;
}

void MessageBoxManager::Show(const std::string &msg,
							 IPropertyObserver *propertyObserver,
							 MessageBox::Buttons buttons)
{
	if (msgBox != NULL)
		delete msgBox;
	
	msgBox = new MessageBox(msg, buttons, Color(38, 62, 119));
	
	if (propertyObserver)
		ObsCast(IPropertyObserver, msgBox)->AddObserver(propertyObserver);
	
	msgBox->Show();
}

MessageBox* MessageBoxManager::GetMessageBox() const
{
	return msgBox;
}

MessageBox::Result MessageBoxManager::GetResult() const
{
	assert(msgBox != NULL);
	
	return msgBox->GetResult();
}

bool MessageBoxManager::IsMsgBoxVisible() const
{
	return (msgBox != NULL && msgBox->IsVisible());
}
