#include "hzpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Hazel/Platform/OpenGL/OpenGLVertexArray.h"
namespace Hazel {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}