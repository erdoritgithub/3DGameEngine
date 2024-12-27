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
		Hazel::Renderer::Clear(0.8f, 0.3f, 0.8f, 1);
		float r = 1.0f;
		float g = 0.0f;
		float b = 1.0f;
		HZ_RENDER_III(r, g, b, {
			Hazel::RendererAPI::Clear(r, g, b, 1.0f);
			});
	}

	virtual void OnImGuiRender() override
	{
		/*ImGui::Begin("Example Window");
		ImGui::Text("Hello World!");
		ImGui::End();*/
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