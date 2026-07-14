#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Math/Ray.h>
#include <Core/Shared/Uuid.h>

namespace Core
{
	class Object;
	class Scene;
	class Mesh;
}

namespace Editor
{
	class CSGBrush;

	class RaycastHit
	{
	public:
		Core::Object* object = nullptr;
		CSGBrush* csgBrush = nullptr;
		size_t csgBrushFaceId = -1;
	};

	class Raycast
	{
	private:
		static bool meshTest(Core::Ray& ray, Core::Mesh* mesh, glm::mat4& mtx, CSGBrush** csgBrush, size_t* faceId);

	public:
		static bool hitTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit);
	};
}