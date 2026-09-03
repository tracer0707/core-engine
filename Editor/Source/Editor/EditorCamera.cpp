#include "EditorCamera.h"

#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/FrameBuffer.h>
#include <Core/Interface/Transform.h>
#include <Core/Math/Mathf.h>

namespace Editor
{
	EditorCamera::EditorCamera(Core::Renderer* renderer)
	{
		_renderer = renderer;
		_transform = new Core::Transform(this);
	}

	EditorCamera::~EditorCamera() {}

	const glm::mat4 EditorCamera::getViewMatrix() const
	{
		glm::vec3 right = _transform->getRight();
		glm::vec3 forward = _transform->getForward();
		glm::vec3 position = _transform->getPosition();

		glm::vec3 at = position + forward;
		glm::vec3 up = glm::cross(right, forward);

		return glm::lookAt(position, at, up);
	}

	const glm::mat4 EditorCamera::getProjectionMatrix() const
	{
		float aspect = (float)_fbWidth / (float)_fbHeight;

		return glm::perspective(glm::radians(_fov), aspect, _near, _far);
	}

	const Core::Ray EditorCamera::getCameraToViewportRay(float x, float y) const
	{
		glm::mat4 view = getViewMatrix();
		glm::mat4 proj = getProjectionMatrix();

		return Core::Mathf::getCameraToViewportRay((float)_fbWidth, (float)_fbHeight, view, proj, x, y);
	}

	const glm::vec3 EditorCamera::worldToScreenPoint(glm::vec3 point) const
	{
		glm::mat4 view = getViewMatrix();
		glm::mat4 proj = getProjectionMatrix();

		glm::vec3 fwd = _transform->getForward();
		glm::vec3 pos = _transform->getPosition();

		return Core::Mathf::worldToScreenPoint((float)_fbWidth, (float)_fbHeight, view, proj, fwd, pos, point);
	}

	const glm::vec3 EditorCamera::screenToWorldPoint(glm::vec3 point) const
	{
		glm::mat4 view = getViewMatrix();
		glm::mat4 proj = getProjectionMatrix();

		return Core::Mathf::screenToWorldPoint((float)_fbWidth, (float)_fbHeight, view, proj, point);
	}
} // namespace Editor