#pragma once

#include "Hazel/Core.h"
#include "Hazel/Core/TimeStep.h"
#include "Events/Event.h"

namespace Hazel
{
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}