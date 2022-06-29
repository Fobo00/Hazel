#include "hzpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		default:
			break;
		case RendererApi::API::None:		HZ_CORE_ASSERT(false, "RenderApi::None is currently not supported!");
		case RendererApi::API::OpenGL:	return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		HZ_CORE_ASSERT(false, "Unknown RenderApi!");
		return nullptr;
	}

}