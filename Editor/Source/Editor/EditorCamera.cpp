#include "EditorCamera.h"

#include <Core/Renderer/Renderer.h>
#include <Core/Interface/Transform.h>
#include <Core/Math/Mathf.h>

namespace Editor
{
	EditorCamera::EditorCamera(Core::Renderer* renderer)
	{
		_renderer = renderer;
		_transform = new Core::Transform();
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
		unsigned int w = _renderer->getWidth();
		unsigned int h = _renderer->getHeight();

		float aspect = (float)w / (float)h;

		return glm::perspective(glm::radians(_fov), aspect, _near, _far);
	}

	const Core::Ray EditorCamera::getCameraToViewportRay(float x, float y) const
	{
		float width = _renderer->getWidth();
		float height = _renderer->getHeight();

		glm::mat4 view = getViewMatrix();
		glm::mat4 proj = getProjectionMatrix();

		return Core::Mathf::getCameraToViewportRay(width, height, view, proj, x, y);
	}

	const glm::vec3 EditorCamera::worldToScreenPoint(glm::vec3 point) const
	{
		float width = _renderer->getWidth();
		float height = _renderer->getHeight();

		glm::mat4 view = getViewMatrix();
		glm::mat4 proj = getProjectionMatrix();

		glm::vec3 fwd = _transform->getForward();
		glm::vec3 pos = _transform->getPosition();

		return Core::Mathf::worldToScreenPoint(width, height, view, proj, fwd, pos, point);
	}

	const glm::vec3 EditorCamera::screenToWorldPoint(glm::vec3 point) const
	{
		float width = _renderer->getWidth();
		float height = _renderer->getHeight();

		glm::mat4 view = getViewMatrix();
		glm::mat4 proj = getProjectionMatrix();

		return Core::Mathf::screenToWorldPoint(width, height, view, proj, point);
	}
} // namespace Editor