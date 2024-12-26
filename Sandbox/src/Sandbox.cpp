#include "Hazel.h"


class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		HZ_TRACE("Hello");
	}
};
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}