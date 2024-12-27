#include "Hazel.h"
#include "Hazel/ImGui/ImGuiLayer.h"

class GameLayer : public Hazel::Layer
{
public:
	GameLayer()
	{
	}
	virtual ~GameLayer()
	{
	}
	virtual void OnAttach() override
	{
	}
	virtual void OnDetach() override
	{
	}
	virtual void OnUpdate() override
	{
		//#define HZ_RENDER(x) ::Hazel::Renderer::Submit([=](){ RendererAPI::x })
		//
		//		Renderer::Clear(0, 0, 0, 1);
		//		HZ_RENDER(Clear(0, 0, 0, 1));
	}
	virtual void OnEvent(Hazel::Event& event) override
	{
	}
};


class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		HZ_TRACE("Hello");
	}

	virtual void OnInit() override
	{
		PushLayer(new GameLayer());
		PushOverlay(new Hazel::ImGuiLayer("ImGui"));
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}