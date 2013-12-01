#include "ScreenKeyboard.h"
#include "IScreenKeyboardObserver.h"
#include <Utils/StringUtils.h>

using namespace Tizen::Ui::Controls;

ScreenKeyboard* ScreenKeyboard::m_instance;

ScreenKeyboard::ScreenKeyboard() :
	m_observer(NULL)
{
}

ScreenKeyboard::~ScreenKeyboard()
{
}

ScreenKeyboard* ScreenKeyboard::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new ScreenKeyboard();
		m_instance->m_keypad = new Keypad();
		m_instance->m_keypad->Construct(KEYPAD_STYLE_NORMAL, KEYPAD_MODE_ALPHA);
		m_instance->m_keypad->SetSingleLineEnabled(true);
		m_instance->m_keypad->AddTextEventListener(*m_instance);
	}

	return m_instance;
}

void ScreenKeyboard::SetObserver(IScreenKeyboardObserver* observer)
{
	m_observer = observer;
}

void ScreenKeyboard::Show()
{
	m_keypad->Show();
}

void ScreenKeyboard::Hide()
{
	// well, there is no hide
}

std::string ScreenKeyboard::GetText()
{
	return StringUtils::ToNarrow(m_keypad->GetText().GetPointer());
}

void ScreenKeyboard::SetText(const std::string& text)
{
	m_keypad->SetText(text.c_str());
}

void ScreenKeyboard::OnTextValueChanged(const Tizen::Ui::Control& source)
{
	if (m_observer != NULL)
		m_observer->ScreenKeyboardDone(StringUtils::ToNarrow(m_keypad->GetText().GetPointer()));
}

void ScreenKeyboard::OnTextValueChangeCanceled(const Tizen::Ui::Control& source)
{
	if (m_observer != NULL)
		m_observer->ScreenKeyboardCanceled();
}
