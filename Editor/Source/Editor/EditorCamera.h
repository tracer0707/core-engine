#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include <Core/Math/Ray.h>

namespace Core
{
	class Renderer;
	class Transform;
	class FrameBuffer;
}

namespace Editor
{
	class EditorCamera
	{
		private:
			Core::Renderer* _renderer = nullptr;
			Core::Transform* _transform = nullptr;
			const Core::FrameBuffer* _frameBuffer = nullptr;

			float _fov = 75.0f;
			float _near = 0.01f;
			float _far = 1000.0f;

		public:
			EditorCamera(Core::Renderer* renderer, const Core::FrameBuffer* frameBuffer);
			~EditorCamera();

			float getFov() const { return _fov; }
			void setFov(float value) { _fov = value; }

			float getNear() const { return _near; }
			void setNear(float value) { _near = value; }

			float getFar() const { return _far; }
			void setFar(float value) { _far = value; }

			void setFrameBuffer(const Core::FrameBuffer* value) { _frameBuffer = value; }
			const Core::FrameBuffer* getFrameBuffer() const { return _frameBuffer; }

			Core::Transform* getTransform() const { return _transform; }

			const glm::mat4 getViewMatrix() const;
			const glm::mat4 getProjectionMatrix() const;

			const Core::Ray getCameraToViewportRay(float x, float y) const;

			const glm::vec3 worldToScreenPoint(glm::vec3 point) const;
			const glm::vec3 screenToWorldPoint(glm::vec3 point) const;
	};
} // namespace Core