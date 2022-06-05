#include "hzpch.h"
#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{
	}


	void OnUpdate() override
	{
		//HZ_INFO("ExampleLayer::Update()");
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB	))
		{
			HZ_TRACE("Key Tab pressed!");
		}
	}

	void OnEvent(Hazel::Event& event) override
	{
		// HZ_INFO("{0}", e.ToString());
		// HZ_TRACE("got Event");
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			HZ_INFO("{0}",(char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}
	~Sandbox()
	{
	}

private:

};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}