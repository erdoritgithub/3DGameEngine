#pragma once
#ifdef HZ_PLATFORM_WINDOWS
extern Hazel::Application* Hazel::CreateApplication();
int main(int argc, char** argv)
{
	hz::Log::Init();
	Hazel::Application* app = Hazel::CreateApplication();
	app->Run();
	delete app;
}
#else
#error Hazel only supports Windows
#endif