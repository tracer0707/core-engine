#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Math/Ray.h>

namespace Core
{
	class Object;
	class Scene;
}

namespace Editor
{
	class RaycastHit
	{
	public:
		Core::Object* object = nullptr;
	};

	class Raycast
	{
	private:
		bool csgTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit);

	public:
		bool hitTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit);
	};
}