#pragma once
#include "Hazel/Input.h"

namespace Hazel
{
	class WindowsInput : public Input
	{
	public:
	protected:

		virtual bool IsKeyPressedImpl(int KeyCode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl();
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}