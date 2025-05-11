#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Math/Ray.h>

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
		unsigned long long brushId = ULONG_MAX;
	};

	class Raycast
	{
	private:
		static bool meshTest(Core::Ray& ray, Core::Mesh* mesh, glm::mat4& mtx, unsigned long long* brushId);

	public:
		static bool hitTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit);
	};
}