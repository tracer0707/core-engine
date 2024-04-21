#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "Component.h"
#include "../Shared/String.h"

namespace Core
{
	class Renderer;
	class Transform;

	class Camera: Component
	{
	private:
		Transform* transform = nullptr;

		float _fov = 75.0f;
		float _near = 0.01f;
		float _far = 1000.0f;

	public:
		Camera();
		virtual ~Camera();

		static UString COMPONENT_TYPE;
		virtual UString getComponentType();

		float getFov() { return _fov; }
		void setFov(float value) { _fov = value; }

		float getNear() { return _near; }
		void setNear(float value) { _near = value; }

		float getFar() { return _far; }
		void setFar(float value) { _far = value; }

		const glm::mat4x4 getViewMatrix();
		const glm::mat4x4 getProjectionMatrix(float aspect);
	};
}