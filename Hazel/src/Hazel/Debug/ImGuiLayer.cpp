#include "hzpch.h"
#include "ImGuiLayer.h"
#include "Hazel/Core/Events/KeyEvent.h"

namespace Hazel {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGUI Layer")
	{
	}
	ImGuiLayer::ImGuiLayer(const std::string& name)
		: Layer(name)
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
	}
	void ImGuiLayer::OnDetach()
	{
	}
	void ImGuiLayer::OnUpdate()
	{
	}
	void ImGuiLayer::OnEvent(Event& event)
	{
		if (GetName() == "Second Layer" && event.IsInCategory(EventCategoryKeyboard))
		{
			event.Handled = true;
		}
		if (GetName() == "Second Layer")
			HZ_INFO("{0}: {1}", GetName(), event.ToString());
		else if (GetName() == "ImGUI Layer")
			HZ_WARN("{0}: {1}", GetName(), event.ToString());
	}
}