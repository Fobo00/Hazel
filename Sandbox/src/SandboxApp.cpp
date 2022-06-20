#include "hzpch.h"
#include <Hazel.h>
#include <ImGui/imgui.h>

static bool showTest = true;

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

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("aha this test works");
		ImGui::DragFloat("Float", new float);
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		// HZ_INFO("{0}", e.ToString());
		// HZ_TRACE("got Event");
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			HZ_TRACE("{0}",(char)e.GetKeyCode());
		}
	}
};

class DrawLayer : public Hazel::Layer
{
public:
	DrawLayer() :Layer("Draw Layer"), m_Camera(-1.6f, 1.6f, -.9f, .9f) {}

	void OnAttach() override
	{
		using namespace Hazel;
		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7]{
			-0.5f, -0.5f, 0.0f, 0.7f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.7f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.7f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		float squareVertices[3 * 7]{
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
		};

		m_SquareVA.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
				{ ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";


		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));


		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjectionMatrix;

			void main()
			{
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_Shader2.reset(new Shader(vertexSrc2, fragmentSrc2));
	}


	void OnDetach() override
	{
	}


	void OnUpdate() override
	{
		using namespace Hazel;
		RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		RenderCommand::Clear();

		//m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
		m_Camera.SetRotation(45.0f);

		Renderer::BeginScene(m_Camera);

		Renderer::Submit(m_Shader2, m_SquareVA);
		Renderer::Submit(m_Shader, m_VertexArray);

		Renderer::EndScene();
	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

	std::shared_ptr<Hazel::Shader> m_Shader2;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new DrawLayer());
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