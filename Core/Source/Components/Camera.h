#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "../Shared/String.h"
#include "../Math/Ray.h"

#include "Component.h"

namespace Core
{
	class Object;

	class Camera: public Component
	{
	private:
		float _fov = 75.0f;
		float _near = 0.01f;
		float _far = 1000.0f;

	public:
		Camera(Object* owner);
		virtual ~Camera();

		virtual UInt32 getComponentType();

		float getFov() { return _fov; }
		void setFov(float value) { _fov = value; }

		float getNear() { return _near; }
		void setNear(float value) { _near = value; }

		float getFar() { return _far; }
		void setFar(float value) { _far = value; }

		const glm::mat4 getViewMatrix();
		const glm::mat4 getProjectionMatrix();

		const Ray getCameraToViewportRay(float screenX, float screenY);

		const glm::vec3 worldToScreenPoint(glm::vec3 point);
		const glm::vec3 screenToWorldPoint(glm::vec3 point);
	};
}