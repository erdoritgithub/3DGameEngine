#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Scene/Entity.h"

#include "Hazel/Core/TimeStep.h"

namespace Hazel {

	enum class ForceMode : uint16_t
	{
		Force = 0,
		Impulse,
		VelocityChange,
		Acceleration
	};

	enum class FilterGroup : uint32_t
	{
		Static = BIT(0),
		Dynamic = BIT(1),
		Kinematic = BIT(2),
		All = Static | Dynamic | Kinematic
	};

	struct SceneParams
	{
		glm::vec3 Gravity = { 0.0F, -9.81F, 0.0F };
	};

	struct RaycastHit
	{
		uint64_t EntityID;
		glm::vec3 Position;
		glm::vec3 Normal;
		float Distance;
	};

	class Physics
	{
	public:
		static void Init();
		static void Shutdown();
		static void ExpandEntityBuffer(uint32_t entityCount);

		static void CreateScene(const SceneParams& params);
		static void CreateActor(Entity e);

		static void SetGravity(float gravity);
		static float GetGravity();

		static void SetFixedTimestep(float timestep);
		static float GetFixedTimestep();

		static void Simulate(Timestep ts);

		static void DestroyScene();
		static void* GetPhysicsScene();

	};

}