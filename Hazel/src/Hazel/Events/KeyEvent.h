#pragma once

#include "Event.h"

namespace Hazel
{

	class HAZEL_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		inline int GetScanCode() const { return m_ScanCode; }
		inline int GetMods() const { return m_Mods; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);
	protected:
		KeyEvent(int keycode, int scanCode, int mods)
			: m_KeyCode(keycode), m_ScanCode(scanCode), m_Mods(mods) {}
		int m_KeyCode;
		int m_ScanCode;
		int m_Mods;
	};

	class HAZEL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int scanCode, int mods, int repeatCount)
			: KeyEvent(keycode, scanCode, mods), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);
	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode, int scanCode, int mods)
			: KeyEvent(keycode, scanCode, mods) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(unsigned int keycode)
			: KeyEvent(keycode, NULL, NULL), m_KeyChar(keycode) {}

		inline unsigned int GetKeyChar() { return m_KeyChar; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped);
	private:
		unsigned int m_KeyChar;
	};

}