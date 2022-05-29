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
		HZ_INFO("ExampleLayer::Update()");
	}

	void OnEvent(Hazel::Event& e) override
	{
		HZ_ERROR("{0}", e.ToString());
		HZ_ERROR("got Event");
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
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