#include "hzpch.h"
#include "WindowsInput.h"
#include "WindowsWindow.h"
#include "Hazel/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hazel {
	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());
		auto state = glfwGetKey(window.GetGLFWWindow(), keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());
		auto state = glfwGetMouseButton(window.GetGLFWWindow(), button);
		return state == GLFW_PRESS;
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());
		double xpos, ypos;
		glfwGetCursorPos(window.GetGLFWWindow(), &xpos, &ypos);
		return (float)xpos;
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());
		double xpos, ypos;
		glfwGetCursorPos(window.GetGLFWWindow(), &xpos, &ypos);
		return (float)ypos;
	}
}