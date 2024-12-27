#include "hzpch.h"
#include "Hazel/Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Hazel {
	VertexBuffer* VertexBuffer::Create(unsigned int size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    return nullptr;
		case RendererAPIType::OpenGL:  return new OpenGLVertexBuffer(size);
		}
		return nullptr;
	}
}