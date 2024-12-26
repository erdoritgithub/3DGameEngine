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
		PushOverlay(new Hazel::ImGuiLayer("ImGui"));
	}
};
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}