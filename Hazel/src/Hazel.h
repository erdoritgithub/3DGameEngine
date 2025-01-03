#pragma once


#include "Hazel/Core/Application.h"
#include <Hazel/Core/Log.h>
#include "Hazel/Core/Input.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Core/Events/Event.h"
#include "Hazel/Core/Events/ApplicationEvent.h"
#include "Hazel/Core/Events/KeyEvent.h"
#include "Hazel/Core/Events/MouseEvent.h"

#include "imgui/imgui.h"

// --- Hazel Render API ------------------------------
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/SceneRenderer.h"
#include "Hazel/Renderer/RenderPass.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Mesh.h"
#include "Hazel/Renderer/Camera.h"
#include "Hazel/Renderer/Material.h"
#include "Hazel/Renderer/Framebuffer.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"

// ---------------------------------------------------
// Scenes
#include "Hazel/Scene/Entity.h"
#include "Hazel/Scene/Scene.h"
