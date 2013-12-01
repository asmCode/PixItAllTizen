#ifndef ISCREENKEYBOARDOBSERVER_H_
#define ISCREENKEYBOARDOBSERVER_H_

#include <string>

class IScreenKeyboardObserver
{
public:
	virtual ~IScreenKeyboardObserver() {}

	virtual void ScreenKeyboardDone(const std::string& text) = 0;
	virtual void ScreenKeyboardCanceled() = 0;
};


#endif /* ISCREENKEYBOARDOBSERVER_H_ */
