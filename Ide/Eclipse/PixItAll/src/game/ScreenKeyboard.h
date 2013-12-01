#ifndef SCREENKEYBOARD_H_
#define SCREENKEYBOARD_H_

#include <FUi.h>
#include <string>

class IScreenKeyboardObserver;

class ScreenKeyboard : public Tizen::Ui::ITextEventListener
{
public:
	virtual ~ScreenKeyboard();

	static ScreenKeyboard* GetInstance();

	void Show();
	void Hide();

	std::string GetText();
	void SetText(const std::string& text);
	void SetObserver(IScreenKeyboardObserver* observer);

private:
	static ScreenKeyboard* m_instance;

	Tizen::Ui::Controls::Keypad* m_keypad;
	IScreenKeyboardObserver* m_observer;

	ScreenKeyboard();

	void OnTextValueChanged(const Tizen::Ui::Control& source);
	void OnTextValueChangeCanceled(const Tizen::Ui::Control& source);
};

#endif /* SCREENKEYBOARD_H_ */
