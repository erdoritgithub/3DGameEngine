#pragma once

#include "Hazel/Core/Math/Transform.h"
#include <PhysX/PxPhysicsAPI.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	physx::PxTransform ToPhysXTransform(const Transform& transform);
	physx::PxTransform ToPhysXTransform(const glm::mat4& transform);

	physx::PxMat44 ToPhysXMatrix(const glm::mat4& matrix);
	physx::PxVec3 ToPhysXVector(const glm::vec3& vector);
	physx::PxVec4 ToPhysXVector(const glm::vec4& vector);
	physx::PxQuat ToPhysXQuat(const glm::quat& quat);

	glm::mat4 FromPhysXTransform(const physx::PxTransform& transform);
	glm::mat4 FromPhysXMatrix(const physx::PxMat44& matrix);
	glm::vec3 FromPhysXVector(const physx::PxVec3& vector);
	glm::vec4 FromPhysXVector(const physx::PxVec4& vector);
	glm::quat FromPhysXQuat(const physx::PxQuat& quat);

	physx::PxFilterFlags HazelFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1,
	physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);

	class ConvexMeshSerializer
	{
	public:
		static void SerializeMesh(const std::string& filepath, const physx::PxDefaultMemoryOutputStream& data);
		static bool IsSerialized(const std::string& filepath);
		static physx::PxDefaultMemoryInputData DeserializeMesh(const std::string& filepath);
	};

	
}