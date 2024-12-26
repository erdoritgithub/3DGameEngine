#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Core/LayerStack.h"

#include "Hazel/Core/Events/ApplicationEvent.h"

namespace Hazel {
	class Application
	{

	public:
		Application();
		virtual ~Application();
		void Run();
		virtual void OnInit() {}
		virtual void OnShutdown() {}
		virtual void OnUpdate() {}

		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }

	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;

	};
	// Implemented by CLIENT
	Application* CreateApplication();
}