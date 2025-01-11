#include "hzpch.h"

#include "ScriptWrappers.h"
#include "Hazel/Core/Math/Noise.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"
#include "Hazel/Scene/Components.h"
#include "Hazel/Physics/PhysicsUtil.h"
#include "Hazel/Physics/PXPhysicsWrappers.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <mono/jit/jit.h>
#include <box2d/box2d.h>
#include <PhysX/PxPhysicsAPI.h>

#include <imgui.h>

namespace Hazel {
	extern std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
	extern std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;
}

namespace Hazel {

	namespace Script {

		enum class ComponentID
		{
			None = 0,
			Transform = 1,
			Mesh = 2,
			Script = 3,
			SpriteRenderer = 4
		};

		static std::tuple<glm::vec3, glm::quat, glm::vec3> GetTransformDecomposition(const glm::mat4& transform)
		{
			glm::vec3 scale, translation, skew;
			glm::vec4 perspective;
			glm::quat orientation;
			glm::decompose(transform, scale, orientation, translation, skew, perspective);
			return { translation, orientation, scale };
		}

		////////////////////////////////////////////////////////////////
		// Math ////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////
		float Hazel_Noise_PerlinNoise(float x, float y)
		{
			return Noise::PerlinNoise(x, y);
		}

		////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////
		// Input ///////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////
		bool Hazel_Input_IsKeyPressed(KeyCode key)
		{
			return Input::IsKeyPressed(key);
		}

		bool Hazel_Input_IsMouseButtonPressed(MouseButton button)
		{
			return Input::IsMouseButtonPressed(button);
		}

		void Hazel_Input_GetMousePosition(glm::vec2* outPosition)
		{
			auto [x, y] = Input::GetMousePosition();
			*outPosition = { x, y };
		}

		void Hazel_Input_SetCursorMode(CursorMode mode)
		{
			Input::SetCursorMode(mode);
		}

		CursorMode Hazel_Input_GetCursorMode()
		{
			return Input::GetCursorMode();
		}

		bool Hazel_Physics_Raycast(glm::vec3* origin, glm::vec3* direction, float maxDistance, RaycastHit* hit)
		{
			return PXPhysicsWrappers::Raycast(*origin, *direction, maxDistance, hit);
		}

		MonoArray* Hazel_Physics_OverlapBox(glm::vec3* origin, glm::vec3* halfSize)
		{
			MonoArray* outColliders = nullptr;
			uint32_t arrayIndex = 0;
			std::vector<physx::PxOverlapHit> buffer;
			if (PXPhysicsWrappers::OverlapBox(*origin, *halfSize, buffer))
			{
				// TODO: Exclude MeshColliders for now
				uintptr_t count = buffer.size();
				for (const auto& hit : buffer)
				{
					Entity& entity = *(Entity*)hit.actor->userData;
					if (!entity.HasComponent<BoxColliderComponent>() && !entity.HasComponent<SphereColliderComponent>() && !entity.HasComponent<CapsuleColliderComponent>())
					{
						if (entity.HasComponent<MeshColliderComponent>())
						{
							count--;
						}
					}
				}
				outColliders = mono_array_new(mono_domain_get(), ScriptEngine::GetCoreClass("Hazel.Collider"), count);
				for (const auto& hit : buffer)
				{
					if (arrayIndex >= count)
						break;
					Entity& entity = *(Entity*)hit.actor->userData;
					if (entity.HasComponent<BoxColliderComponent>())
					{
						auto& boxCollider = entity.GetComponent<BoxColliderComponent>();
						void* data[] = {
							&entity.GetUUID(),
							&boxCollider.IsTrigger,
							&boxCollider.Size,
							&boxCollider.Offset
						};
						MonoObject* obj = ScriptEngine::Construct("Hazel.BoxCollider:.ctor(ulong,bool,Vector3,Vector3)", true, data);
						mono_array_set(outColliders, MonoObject*, arrayIndex++, obj);
					}
					if (entity.HasComponent<SphereColliderComponent>())
					{
						auto& sphereCollider = entity.GetComponent<SphereColliderComponent>();
						void* data[] = {
							&entity.GetUUID(),
							&sphereCollider.IsTrigger,
							&sphereCollider.Radius
						};
						MonoObject* obj = ScriptEngine::Construct("Hazel.SphereCollider:.ctor(ulong,bool,float)", true, data);
						mono_array_set(outColliders, MonoObject*, arrayIndex++, obj);
					}
					if (entity.HasComponent<CapsuleColliderComponent>())
					{
						auto& capsuleCollider = entity.GetComponent<CapsuleColliderComponent>();
						void* data[] = {
							&entity.GetUUID(),
							&capsuleCollider.IsTrigger,
							&capsuleCollider.Radius,
							&capsuleCollider.Height
						};
						MonoObject* obj = ScriptEngine::Construct("Hazel.CapsuleCollider:.ctor(ulong,bool,float,float)", true, data);
						mono_array_set(outColliders, MonoObject*, arrayIndex++, obj);
					}
					/*if (entity.HasComponent<MeshColliderComponent>())
					{
						auto& meshCollider = entity.GetComponent<MeshColliderComponent>();
						MonoString* filepath = mono_string_new(mono_domain_get(), meshCollider.CollisionMesh->GetFilePath().c_str());
						void* data[] = {
							&entity.GetUUID(),
							&meshCollider.IsTrigger,
							&filepath
						};
						MonoObject* obj = ScriptEngine::Construct("Hazel.MeshCollider:.ctor(ulong,bool,string)", true, data);
						mono_array_set(outColliders, MonoObject*, arrayIndex++, obj);
					}*/
				}
			}
			return outColliders;
		}

		MonoArray* Hazel_Physics_OverlapSphere(glm::vec3* origin, float radius)
		{
			MonoArray* outColliders = nullptr;
			uint32_t arrayIndex = 0;
			std::vector<physx::PxOverlapHit> buffer;
			if (PXPhysicsWrappers::OverlapSphere(*origin, radius, buffer))
			{
				// TODO: Exclude MeshColliders for now
				uintptr_t count = buffer.size();
				for (const auto& hit : buffer)
				{
					Entity& entity = *(Entity*)hit.actor->userData;
					if (!entity.HasComponent<BoxColliderComponent>() && !entity.HasComponent<SphereColliderComponent>() && !entity.HasComponent<CapsuleColliderComponent>())
					{
						if (entity.HasComponent<MeshColliderComponent>())
						{
							count--;
						}
					}
				}
				outColliders = mono_array_new(mono_domain_get(), ScriptEngine::GetCoreClass("Hazel.Collider"), count);
				for (const auto& hit : buffer)
				{
					if (arrayIndex >= count)
						break;
					Entity& entity = *(Entity*)hit.actor->userData;
					if (entity.HasComponent<BoxColliderComponent>())
					{
						auto& boxCollider = entity.GetComponent<BoxColliderComponent>();
						void* data[] = {
							&entity.GetUUID(),
							&boxCollider.IsTrigger,
							&boxCollider.Size,
							&boxCollider.Offset
						};
						MonoObject* obj = ScriptEngine::Construct("Hazel.BoxCollider:.ctor(ulong,bool,Vector3,Vector3)", true, data);
						mono_array_set(outColliders, MonoObject*, arrayIndex++, obj);
					}
					if (entity.HasComponent<SphereColliderComponent>())
					{
						auto& sphereCollider = entity.GetComponent<SphereColliderComponent>();
						void* data[] = {
							&entity.GetUUID(),
							&sphereCollider.IsTrigger,
							&sphereCollider.Radius
						};
						MonoObject* obj = ScriptEngine::Construct("Hazel.SphereCollider:.ctor(ulong,bool,float)", true, data);
						mono_array_set(outColliders, MonoObject*, arrayIndex++, obj);
					}
					if (entity.HasComponent<CapsuleColliderComponent>())
					{
						auto& capsuleCollider = entity.GetComponent<CapsuleColliderComponent>();
						void* data[] = {
							&entity.GetUUID(),
							&capsuleCollider.IsTrigger,
							&capsuleCollider.Radius,
							&capsuleCollider.Height
						};
						MonoObject* obj = ScriptEngine::Construct("Hazel.CapsuleCollider:.ctor(ulong,bool,float,float)", true, data);
						mono_array_set(outColliders, MonoObject*, arrayIndex++, obj);
					}
					/*if (entity.HasComponent<MeshColliderComponent>())
					{
						auto& meshCollider = entity.GetComponent<MeshColliderComponent>();
						MonoString* filepath = mono_string_new(mono_domain_get(), meshCollider.CollisionMesh->GetFilePath().c_str());
						void* data[] = {
							&entity.GetUUID(),
							&meshCollider.IsTrigger,
							&filepath
						};
						MonoObject* obj = ScriptEngine::Construct("Hazel.MeshCollider:.ctor(ulong,bool,string)", true, data);
						mono_array_set(outColliders, MonoObject*, arrayIndex++, obj);
					}*/
				}
			}
			return outColliders;
		}

		////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////
		// Entity //////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////
		void Hazel_Entity_GetTransform(uint64_t entityID, glm::mat4* outTransform)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			memcpy(outTransform, glm::value_ptr(transformComponent.Transform), sizeof(glm::mat4));
		}

		void Hazel_Entity_SetTransform(uint64_t entityID, glm::mat4* inTransform)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			memcpy(glm::value_ptr(transformComponent.Transform), inTransform, sizeof(glm::mat4));
		}

		void Hazel_Entity_CreateComponent(uint64_t entityID, void* type)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
			s_CreateComponentFuncs[monoType](entity);
		}

		bool Hazel_Entity_HasComponent(uint64_t entityID, void* type)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
			bool result = s_HasComponentFuncs[monoType](entity);

			return result;
		}

		uint64_t Hazel_Entity_FindEntityByTag(MonoString* tag)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			Entity entity = scene->FindEntityByTag(mono_string_to_utf8(tag));
			if (entity)
				return entity.GetComponent<IDComponent>().ID;

			return 0;
		}

		void Hazel_TransformComponent_GetRelativeDirection(uint64_t entityID, glm::vec3* outDirection, glm::vec3* inAbsoluteDirection)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			auto [position, rotation, scale] = GetTransformDecomposition(transformComponent.Transform);
			*outDirection = glm::rotate(rotation, *inAbsoluteDirection);
		}
		void Hazel_TransformComponent_GetRotation(uint64_t entityID, glm::vec3* outRotation)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			auto [position, rotationQuat, scale] = GetTransformDecomposition(transformComponent.Transform);
			*outRotation = glm::degrees(glm::eulerAngles(rotationQuat));
		}

		void Hazel_TransformComponent_SetRotation(uint64_t entityID, glm::vec3* inRotation)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			glm::mat4& transform = entity.Transform();

			auto [translation, rotationQuat, scale] = GetTransformDecomposition(transform);
			transform = glm::translate(glm::mat4(1.0F), translation) *
				glm::toMat4(glm::quat(glm::radians(*inRotation))) *
				glm::scale(glm::mat4(1.0F), scale);
		}

		void* Hazel_MeshComponent_GetMesh(uint64_t entityID)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			auto& meshComponent = entity.GetComponent<MeshComponent>();
			return new Ref<Mesh>(meshComponent.Mesh);
		}

		void Hazel_MeshComponent_SetMesh(uint64_t entityID, Ref<Mesh>* inMesh)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			auto& meshComponent = entity.GetComponent<MeshComponent>();
			meshComponent.Mesh = inMesh ? *inMesh : nullptr;
		}

		void Hazel_RigidBody2DComponent_ApplyLinearImpulse(uint64_t entityID, glm::vec2* impulse, glm::vec2* offset, bool wake)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBody2DComponent>());
			auto& component = entity.GetComponent<RigidBody2DComponent>();
			b2Body* body = (b2Body*)component.RuntimeBody;
			body->ApplyLinearImpulse(*(const b2Vec2*)impulse, body->GetWorldCenter() + *(const b2Vec2*)offset, wake);
		}

		void Hazel_RigidBody2DComponent_GetLinearVelocity(uint64_t entityID, glm::vec2* outVelocity)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBody2DComponent>());
			auto& component = entity.GetComponent<RigidBody2DComponent>();
			b2Body* body = (b2Body*)component.RuntimeBody;
			const auto& velocity = body->GetLinearVelocity();
			HZ_CORE_ASSERT(outVelocity);
			*outVelocity = { velocity.x, velocity.y };
		}

		void Hazel_RigidBody2DComponent_SetLinearVelocity(uint64_t entityID, glm::vec2* velocity)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBody2DComponent>());
			auto& component = entity.GetComponent<RigidBody2DComponent>();
			b2Body* body = (b2Body*)component.RuntimeBody;
			HZ_CORE_ASSERT(velocity);
			body->SetLinearVelocity({ velocity->x, velocity->y });
		}

		void Hazel_RigidBodyComponent_AddForce(uint64_t entityID, glm::vec3* force, ForceMode forceMode)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();
			if (component.IsKinematic)
			{
				HZ_CORE_WARN("Cannot add a force to a kinematic actor! EntityID({0})", entityID);
				return;
			}

			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();

			HZ_CORE_ASSERT(dynamicActor);
			HZ_CORE_ASSERT(force);
			dynamicActor->addForce({ force->x, force->y, force->z }, (physx::PxForceMode::Enum)forceMode);
		}

		void Hazel_RigidBodyComponent_AddTorque(uint64_t entityID, glm::vec3* torque, ForceMode forceMode)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();

			if (component.IsKinematic)
			{
				HZ_CORE_WARN("Cannot add torque to a kinematic actor! EntityID({0})", entityID);
				return;
			}

			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();

			HZ_CORE_ASSERT(dynamicActor);
			HZ_CORE_ASSERT(torque);
			dynamicActor->addTorque({ torque->x, torque->y, torque->z }, (physx::PxForceMode::Enum)forceMode);

		}

		void Hazel_RigidBodyComponent_GetLinearVelocity(uint64_t entityID, glm::vec3* outVelocity)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();
			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();

			HZ_CORE_ASSERT(dynamicActor);
			HZ_CORE_ASSERT(outVelocity);

			physx::PxVec3 velocity = dynamicActor->getLinearVelocity();
			*outVelocity = { velocity.x, velocity.y, velocity.z };
		}

		void Hazel_RigidBodyComponent_SetLinearVelocity(uint64_t entityID, glm::vec3* velocity)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();
			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();

			HZ_CORE_ASSERT(dynamicActor);
			HZ_CORE_ASSERT(velocity);

			dynamicActor->setLinearVelocity({ velocity->x, velocity->y, velocity->z });
		}

		void Hazel_RigidBodyComponent_Rotate(uint64_t entityID, glm::vec3* rotation)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());

			auto& component = entity.GetComponent<RigidBodyComponent>();
			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();
			HZ_CORE_ASSERT(dynamicActor);

			physx::PxTransform transform = dynamicActor->getGlobalPose();
			transform.q *= (physx::PxQuat(glm::radians(rotation->x), { 1.0F, 0.0F, 0.0F })
				* physx::PxQuat(glm::radians(rotation->y), { 0.0F, 1.0F, 0.0F })
				* physx::PxQuat(glm::radians(rotation->z), { 0.0F, 0.0F, 1.0F }));
			dynamicActor->setGlobalPose(transform);

		}

		uint32_t Hazel_RigidBodyComponent_GetLayer(uint64_t entityID)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();
			return component.Layer;
		}

		float Hazel_RigidBodyComponent_GetMass(uint64_t entityID)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();
			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();
			HZ_CORE_ASSERT(dynamicActor);
			return dynamicActor->getMass();
		}

		void Hazel_RigidBodyComponent_SetMass(uint64_t entityID, float mass)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			HZ_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			HZ_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			Entity entity = entityMap.at(entityID);
			HZ_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();
			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();
			HZ_CORE_ASSERT(dynamicActor);
			component.Mass = mass;
			physx::PxRigidBodyExt::updateMassAndInertia(*dynamicActor, mass);
		}

		Ref<Mesh>* Hazel_Mesh_Constructor(MonoString* filepath)
		{
			return new Ref<Mesh>(new Mesh(mono_string_to_utf8(filepath)));
		}

		void Hazel_Mesh_Destructor(Ref<Mesh>* _this)
		{
			Ref<Mesh>* instance = (Ref<Mesh>*)_this;
			delete _this;
		}

		Ref<Material>* Hazel_Mesh_GetMaterial(Ref<Mesh>* inMesh)
		{
			Ref<Mesh>& mesh = *(Ref<Mesh>*)inMesh;
			return new Ref<Material>(mesh->GetMaterial());
		}

		Ref<MaterialInstance>* Hazel_Mesh_GetMaterialByIndex(Ref<Mesh>* inMesh, int index)
		{
			Ref<Mesh>& mesh = *(Ref<Mesh>*)inMesh;
			const auto& materials = mesh->GetMaterials();

			HZ_CORE_ASSERT(index < materials.size());
			return new Ref<MaterialInstance>(materials[index]);
		}

		int Hazel_Mesh_GetMaterialCount(Ref<Mesh>* inMesh)
		{
			Ref<Mesh>& mesh = *(Ref<Mesh>*)inMesh;
			const auto& materials = mesh->GetMaterials();
			return materials.size();
		}

		void* Hazel_Texture2D_Constructor(uint32_t width, uint32_t height)
		{
			auto result = Texture2D::Create(TextureFormat::RGBA, width, height);
			return new Ref<Texture2D>(result);
		}

		void Hazel_Texture2D_Destructor(Ref<Texture2D>* _this)
		{
			delete _this;
		}

		void Hazel_Texture2D_SetData(Ref<Texture2D>* _this, MonoArray* inData, int32_t count)
		{
			Ref<Texture2D>& instance = *_this;

			uint32_t dataSize = count * sizeof(glm::vec4) / 4;
			instance->Lock();
			Buffer buffer = instance->GetWriteableBuffer();
			HZ_CORE_ASSERT(dataSize <= buffer.Size);
			// Convert RGBA32F color to RGBA8
			uint8_t* pixels = (uint8_t*)buffer.Data;
			uint32_t index = 0;
			for (int i = 0; i < instance->GetWidth() * instance->GetHeight(); i++)
			{
				glm::vec4& value = mono_array_get(inData, glm::vec4, i);
				*pixels++ = (uint32_t)(value.x * 255.0f);
				*pixels++ = (uint32_t)(value.y * 255.0f);
				*pixels++ = (uint32_t)(value.z * 255.0f);
				*pixels++ = (uint32_t)(value.w * 255.0f);
			}
			instance->Unlock();
		}

		void Hazel_Material_Destructor(Ref<Material>* _this)
		{
			delete _this;
		}

		void Hazel_Material_SetFloat(Ref<Material>* _this, MonoString* uniform, float value)
		{
			Ref<Material>& instance = *(Ref<Material>*)_this;
			instance->Set(mono_string_to_utf8(uniform), value);
		}

		void Hazel_MaterialInstance_SetVector4(Ref<MaterialInstance>* _this, MonoString* uniform, glm::vec4* value)
		{
			Ref<MaterialInstance>& instance = *(Ref<MaterialInstance>*)_this;
			instance->Set(mono_string_to_utf8(uniform), *value);
		}

		void Hazel_Material_SetTexture(Ref<Material>* _this, MonoString* uniform, Ref<Texture2D>* texture)
		{
			Ref<Material>& instance = *(Ref<Material>*)_this;
			instance->Set(mono_string_to_utf8(uniform), *texture);
		}

		void Hazel_MaterialInstance_Destructor(Ref<MaterialInstance>* _this)
		{
			delete _this;
		}

		void Hazel_MaterialInstance_SetFloat(Ref<MaterialInstance>* _this, MonoString* uniform, float value)
		{
			Ref<MaterialInstance>& instance = *(Ref<MaterialInstance>*)_this;
			instance->Set(mono_string_to_utf8(uniform), value);
		}

		void Hazel_MaterialInstance_SetVector3(Ref<MaterialInstance>* _this, MonoString* uniform, glm::vec3* value)
		{
			Ref<MaterialInstance>& instance = *(Ref<MaterialInstance>*)_this;
			instance->Set(mono_string_to_utf8(uniform), *value);
		}

		void Hazel_MaterialInstance_SetTexture(Ref<MaterialInstance>* _this, MonoString* uniform, Ref<Texture2D>* texture)
		{
			Ref<MaterialInstance>& instance = *(Ref<MaterialInstance>*)_this;
			instance->Set(mono_string_to_utf8(uniform), *texture);
		}

		void* Hazel_MeshFactory_CreatePlane(float width, float height)
		{
			// TODO: Implement properly with MeshFactory class!
			return new Ref<Mesh>(new Mesh("assets/models/Plane1m.obj"));
		}
		////////////////////////////////////////////////////////////////

	}
}