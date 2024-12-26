#include "Hazel.h"
#include "Hazel/Debug/ImGuiLayer.h"

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		HZ_TRACE("Hello");
	}

	virtual void OnInit() override
	{
		PushLayer(new Hazel::ImGuiLayer());
		PushOverlay(new Hazel::ImGuiLayer("Second Layer"));
	}
};
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}