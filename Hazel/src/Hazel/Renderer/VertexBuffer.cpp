#include "hzpch.h"
#include "VertexBuffer.h"

#include "Renderer.h"

#include "Hazel/Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Hazel {

	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, VertexBufferUsage usage)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    return nullptr;
		case RendererAPIType::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(data, size, usage);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    return nullptr;
		case RendererAPIType::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(size, usage);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}