#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel
{

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		default:
			break;
		case RendererApi::API::None:		HZ_CORE_ASSERT(false, "RenderApi::None is currently not supported!");
		case RendererApi::API::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
		}

		HZ_CORE_ASSERT(false, "Unknown RenderApi!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		default:
			break;
		case RendererApi::API::None:		HZ_CORE_ASSERT(false, "RenderApi::None is currently not supported!");
		case RendererApi::API::OpenGL:		return new OpenGLIndexBuffer(indices, size);
		}

		HZ_CORE_ASSERT(false, "Unknown RenderApi!");
		return nullptr;
	}

}

