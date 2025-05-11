#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Math/Ray.h>
#include <Shared/Uuid.h>

namespace Core
{
	class Object;
	class Scene;
	class Mesh;
}

namespace Editor
{
	class RaycastHit
	{
	public:
		Core::Object* object = nullptr;
		Core::Uuid brushId;
	};

	class Raycast
	{
	private:
		static bool meshTest(Core::Ray& ray, Core::Mesh* mesh, glm::mat4& mtx, Core::Uuid* brushId);

	public:
		static bool hitTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit);
	};
}