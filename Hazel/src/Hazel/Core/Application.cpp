#include "hzpch.h"
#include "Application.h"

#include "Hazel/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace Hazel {

#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer("ImGui");
		PushOverlay(m_ImGuiLayer);

		Renderer::Init();
	}

	Application::~Application()
	{
	}

	void Application::RenderImGui()
	{
		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		OnInit();
		while (m_Running)
		{
	
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// Render ImGui on render thread
			Application* app = this;
			HZ_RENDER_1(app, { app->RenderImGui(); });

			Renderer::Get().WaitAndRender();
			
			m_Window->OnUpdate();

		}
		OnShutdown();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		HZ_CORE_INFO("Window Resize: {0}, {1}", e.GetWidth(), e.GetHeight());
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}