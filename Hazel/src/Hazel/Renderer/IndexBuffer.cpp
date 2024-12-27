#include "hzpch.h"

#include "Hazel/Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Hazel {
	IndexBuffer* IndexBuffer::Create(unsigned int size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    return nullptr;
		case RendererAPIType::OpenGL:  return new OpenGLIndexBuffer(size);
		}
		return nullptr;
	}
}