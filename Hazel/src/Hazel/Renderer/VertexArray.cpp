#include "hzpch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Renderer.h"

namespace Hazel
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		default:
			break;
		case RendererApi::API::None:		HZ_CORE_ASSERT(false, "RenderApi::None is currently not supported!");
		case RendererApi::API::OpenGL:	return new OpenGLVertexArray();
		}

		HZ_CORE_ASSERT(false, "Unknown RenderApi!");
		return nullptr;
	}
}
