#pragma once
#include "Hazel/Renderer/Texture.h"
namespace Hazel {
	class ObjectsPanel
	{
	public:
		ObjectsPanel();
		void OnImGuiRender();
	private:
		Ref<Texture2D> m_CubeImage;
	};
}