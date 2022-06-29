#include "hzpch.h"

#include <Hazel.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>

static bool showTest = true;

class ExampleLayer : public Hazel::Layer
{

public:
	void OnEvent(Hazel::Event& event) override
	{
		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	}

private:
	bool OnKeyPressed(Hazel::KeyPressedEvent& e)
	{
		if (e.GetKeyCode()==HZ_KEY_LEFT)
		{
			HZ_INFO("HAHA");
		}
		return false;
	}
};

class DrawLayer : public Hazel::Layer
{
public:
	DrawLayer() :Layer("Draw Layer"), m_Camera(-1.6f, 1.6f, -.9f, .9f), m_UColor(0.0f) {}

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
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
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
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
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


		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));


		std::string flatColorVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			}
		)";

		m_FlatColorShader.reset(Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));
	}


	void OnDetach() override
	{
	}


	void OnUpdate(Hazel::TimeStep ts) override
	{
		HZ_TRACE("Delta time: {0}s [{1}ms]", ts.GetSeconds(), ts.GetMilliSeconds());

		// Camera Transform
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPos.x -= m_MoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPos.x += m_MoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPos.y -= m_MoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPos.y += m_MoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_E))
			m_CameraRotation -= m_RotSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_Q))
			m_CameraRotation += m_RotSpeed * ts;



		// Rendering

		Hazel::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPos);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(.1f));

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		for (int x = 0; x < 20; x++)
		{
			for (int i = 0; i < 20; i++)
			{
				glm::vec3 pos(i * .11f, x * .11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				if (i % 2 == 0) std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", redColor);
				else std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_UColor);

				Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}


	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("ColorPicker", &m_UColor.x);
		ImGui::End();

		ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Text");
	}

private:
	bool OnKey(Hazel::KeyPressedEvent& event)
	{
		return false;
	}
private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

	std::shared_ptr<Hazel::Shader> m_FlatColorShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPos{};
	float m_CameraRotation = 0.0f;
	float m_MoveSpeed = 5.0f;
	float m_RotSpeed = 180.0f;

	glm::vec4 m_UColor;
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