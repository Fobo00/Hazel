#pragma once
#include "Hazel/Renderer/RendererApi.h"

namespace Hazel
{
	class OpenGLRendererAPI : public RendererApi
	{
	public:


		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	};
}