#ifndef SCREENKEYBOARD_H_
#define SCREENKEYBOARD_H_

#include <FUi.h>
#include <string>

class ScreenKeyboard
{
public:
	virtual ~ScreenKeyboard();

	static ScreenKeyboard* GetInstance();

	void Show();
	void Hide();

	std::string GetText();
	void SetText(const std::string& text);

private:
	static ScreenKeyboard* m_instance;

	Tizen::Ui::Controls::Keypad* m_keypad;

	ScreenKeyboard();
};

#endif /* SCREENKEYBOARD_H_ */
