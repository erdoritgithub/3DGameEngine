#pragma once

#include "hzpch.h"
#include "Hazel/Core/Layer.h"

namespace Hazel {
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		virtual ~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
	private:
		float m_Time = 0.0f;
	};
}