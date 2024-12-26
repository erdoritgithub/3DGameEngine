#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
	};
	// Implemented by CLIENT
	Application* CreateApplication();
}