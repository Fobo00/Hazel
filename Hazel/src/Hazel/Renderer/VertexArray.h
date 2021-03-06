#pragma once
#include <memory>
#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() = 0;

		static VertexArray* Create();
	};
}

