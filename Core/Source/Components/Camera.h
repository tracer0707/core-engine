#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace Core
{
	class Renderer;

	class Camera
	{
	private:
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat rotation = glm::identity<glm::quat>();
		float fov = 75.0f;
		float near = 0.01f;
		float far = 1000.0f;

	public:
		Camera() = default;

		glm::vec3 getPosition() { return position; }
		void setPosition(glm::vec3 value) { position = value; }

		glm::quat getRotation() { return rotation; }
		void setRotation(glm::quat value) { rotation = value; }

		float getFov() { return fov; }
		void setFov(float value) { fov = value; }

		float getNear() { return near; }
		void setNear(float value) { near = value; }

		float getFar() { return far; }
		void setFar(float value) { far = value; }

		const glm::mat4x4 getViewMatrix();
		const glm::mat4x4 getProjectionMatrix(float aspect);
	};
}