#include "hzpch.h"

#include "Renderer.h"
#include "RenderCommand.h"

namespace Hazel {
	Renderer* Renderer::s_Instance = new Renderer();

	void Renderer::Init()
	{
	}

	void Renderer::Clear()
	{
		// HZ_RENDER(Clear());
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		float params[4] = { r, g, b, a };
		s_Instance->m_CommandQueue.SubmitCommand(RenderCommand::Clear, params, sizeof(float) * 4);
	}

	void Renderer::ClearMagenta()
	{
		Clear(1, 0, 1);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
		// HZ_RENDER(SetClearColor(r, g, b, a));
	}

	void Renderer::WaitAndRender()
	{
		s_Instance->m_CommandQueue.Execute();
	}

}