#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel
{
	RendererApi* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
