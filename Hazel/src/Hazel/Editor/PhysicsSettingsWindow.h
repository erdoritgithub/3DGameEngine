#pragma once
namespace Hazel {
	class PhysicsSettingsWindow
	{
	public:
		static void OnImGuiRender(bool* show);
		static void RenderLayerList();
		static void RenderSelectedLayer();
	};
}