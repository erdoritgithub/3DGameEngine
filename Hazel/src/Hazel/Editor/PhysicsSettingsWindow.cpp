#include "hzpch.h"
#include "PhysicsSettingsWindow.h"
#include "Hazel/Physics/Physics.h"
#include "Hazel/Physics/PhysicsLayer.h"
#include "imgui.h"
#include "imgui_internal.h"

#include <glm/gtc/type_ptr.hpp>

namespace Hazel {
	static int32_t s_SelectedLayer = -1;
	static char s_NewLayerNameBuffer[50];

	void PhysicsSettingsWindow::OnImGuiRender(bool& show)
	{
		if (!show)
			return;
		ImGui::Begin("Physics", &show);
		ImGui::PushID(0);

		ImGui::Columns(2);
		RenderWorldSettings();
		ImGui::EndColumns();
		ImGui::PopID();

		ImGui::Separator();

		ImGui::PushID(1);
		ImGui::Columns(2);
		RenderLayerList();
		ImGui::NextColumn();
		RenderSelectedLayer();
		ImGui::EndColumns();
		ImGui::PopID();

		ImGui::End();
	}

	void PhysicsSettingsWindow::RenderWorldSettings()
	{
		PhysicsSettings& settings = Physics::GetSettings();
		Property("Fixed Timestep (Default: 0.02)", settings.FixedTimestep);
		Property("Gravity (Default: -9.81)", settings.Gravity.y);

		static const char* broadphaseTypeStrings[] = { "Sweep And Prune", "Multi Box Pruning", "Automatic Box Pruning" };
		Property("Broadphase Type", broadphaseTypeStrings, 3, (int*)&settings.BroadphaseAlgorithm);
		if (settings.BroadphaseAlgorithm != BroadphaseType::AutomaticBoxPrune)
		{
			Property("World Bounds (Min)", settings.WorldBoundsMin);
			Property("World Bounds (Max)", settings.WorldBoundsMax);
			Property("Grid Subdivisions", settings.WorldBoundsSubdivisions, 1.0F, 10000.0F);
		}

		static const char* frictionTypeStrings[] = { "Patch", "One Directional", "Two Directional" };
		Property("Friction Model", frictionTypeStrings, 3, (int*)&settings.FrictionModel);
	}

	void PhysicsSettingsWindow::RenderLayerList()
	{
		if (ImGui::Button("New Layer"))
		{
			ImGui::OpenPopup("NewLayerNamePopup");
		}
		if (ImGui::BeginPopup("NewLayerNamePopup"))
		{
			ImGui::InputText("##LayerNameID", s_NewLayerNameBuffer, 50);
			if (ImGui::Button("Add"))
			{
				PhysicsLayerManager::AddLayer(std::string(s_NewLayerNameBuffer));
				memset(s_NewLayerNameBuffer, 0, 50);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		uint32_t buttonId = 0;
		for (const auto& layer : PhysicsLayerManager::GetLayers())
		{
			if (ImGui::Button(layer.Name.c_str()))
			{
				s_SelectedLayer = layer.LayerID;
			}
			if (layer.Name != "Default")
			{
				ImGui::SameLine();
				ImGui::PushID(buttonId++);
				if (ImGui::Button("X"))
				{
					PhysicsLayerManager::RemoveLayer(layer.LayerID);
				}
				ImGui::PopID();
			}
		}
	}

	static std::string s_IDString = "##";
	void PhysicsSettingsWindow::RenderSelectedLayer()
	{
		if (s_SelectedLayer == -1)
			return;
		const PhysicsLayer& layerInfo = PhysicsLayerManager::GetLayer(s_SelectedLayer);
		for (const auto& layer : PhysicsLayerManager::GetLayers())
		{
			if (layer.LayerID == s_SelectedLayer)
				continue;
			const PhysicsLayer& otherLayerInfo = PhysicsLayerManager::GetLayer(layer.LayerID);
			bool shouldCollide;
			if (layerInfo.CollidesWith == 0 || otherLayerInfo.CollidesWith == 0)
			{
				shouldCollide = false;
			}
			else
			{
				shouldCollide = layerInfo.CollidesWith & otherLayerInfo.BitValue;
			}
			ImGui::TextUnformatted(otherLayerInfo.Name.c_str());
			ImGui::SameLine();
			if (ImGui::Checkbox((s_IDString + otherLayerInfo.Name).c_str(), &shouldCollide))
			{
				PhysicsLayerManager::SetLayerCollision(s_SelectedLayer, layer.LayerID, shouldCollide);
			}
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
		{
			s_SelectedLayer = -1;
		}
	}

	bool PhysicsSettingsWindow::Property(const char* label, float& value, float min, float max)
	{
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		std::string id = "##" + std::string(label);
		bool changed = ImGui::DragFloat(id.c_str(), &value, 1.0F, min, max);
		ImGui::PopItemWidth();
		ImGui::NextColumn();
		return changed;
	}

	bool PhysicsSettingsWindow::Property(const char* label, uint32_t& value, uint32_t min, uint32_t max)
	{
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		std::string id = "##" + std::string(label);
		bool changed = ImGui::DragInt(id.c_str(), (int*)&value, 1.0F, min, max);
		ImGui::PopItemWidth();
		ImGui::NextColumn();
		return changed;
	}

	bool PhysicsSettingsWindow::Property(const char* label, glm::vec3& value, float min, float max)
	{
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		std::string id = "##" + std::string(label);
		bool changed = ImGui::DragFloat3(id.c_str(), glm::value_ptr(value), 1.0F, min, max);
		ImGui::PopItemWidth();
		ImGui::NextColumn();
		return changed;
	}

	bool PhysicsSettingsWindow::Property(const char* label, const char** options, int32_t optionCount, int32_t* selected)
	{
		const char* current = options[*selected];
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		bool changed = false;
		std::string id = "##" + std::string(label);
		if (ImGui::BeginCombo(id.c_str(), current))
		{
			for (int i = 0; i < optionCount; i++)
			{
				bool is_selected = (current == options[i]);
				if (ImGui::Selectable(options[i], is_selected))
				{
					current = options[i];
					*selected = i;
					changed = true;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return changed;
	}

}