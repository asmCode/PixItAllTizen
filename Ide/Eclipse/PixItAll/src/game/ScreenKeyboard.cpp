#include "ScreenKeyboard.h"
#include <Utils/StringUtils.h>

using namespace Tizen::Ui::Controls;

ScreenKeyboard* ScreenKeyboard::m_instance;

ScreenKeyboard::ScreenKeyboard()
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
	}

	return m_instance;
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

