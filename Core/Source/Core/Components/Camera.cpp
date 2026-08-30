#include "Camera.h"

#include "../Interface/Transform.h"

#include "../Renderer/Renderer.h"
#include "../Content/RenderTexture.h"
#include "../Math/Mathf.h"
#include "../Math/Plane.h"
#include "../Scene/Object.h"

namespace Core
{
	Camera::Camera(Object* owner, Renderer* renderer) : Component(owner)
	{
		_renderer = renderer;
	}

	Camera::~Camera() {}

	const glm::mat4 Camera::getViewMatrix() const
	{
		Transform* transform = _owner->getTransform();

		if (transform == nullptr) return glm::identity<glm::mat4>();

		glm::vec3 right = transform->getRight();
		glm::vec3 forward = transform->getForward();
		glm::vec3 position = transform->getPosition();

		glm::vec3 at = position + forward;
		glm::vec3 up = glm::cross(right, forward);

		return glm::lookAt(position, at, up);
	}

	const glm::mat4 Camera::getProjectionMatrix() const
	{
		unsigned int w = _renderer->getWidth();
		unsigned int h = _renderer->getHeight();

		if (renderTexture != nullptr)
		{
			w = renderTexture->getWidth();
			h = renderTexture->getHeight();
		}

		float aspect = (float)w / (float)h;

		return glm::perspective(glm::radians(_fov), aspect, _near, _far);
	}

	const Ray Camera::getCameraToViewportRay(float x, float y) const
	{
		float width = (float)_renderer->getWidth();
		float height = (float)_renderer->getHeight();

		if (renderTexture != nullptr)
		{
			width = (float)renderTexture->getWidth();
			height = (float)renderTexture->getHeight();
		}

		glm::mat4 view = getViewMatrix();
		glm::mat4 proj = getProjectionMatrix();

		return Core::Mathf::getCameraToViewportRay(width, height, view, proj, x, y);
	}

	const glm::vec3 Camera::worldToScreenPoint(glm::vec3 point) const
	{
		Transform* transform = _owner->getTransform();
		if (transform == nullptr) return glm::vec3(0.0f);

		float width = _renderer->getWidth();
		float height = _renderer->getHeight();

		if (renderTexture != nullptr)
		{
			width = renderTexture->getWidth();
			height = renderTexture->getHeight();
		}

		glm::mat4 view = getViewMatrix();
		glm::mat4 proj = getProjectionMatrix();

		glm::vec3 fwd = transform->getForward();
		glm::vec3 pos = transform->getPosition();

		return Core::Mathf::worldToScreenPoint(width, height, view, proj, fwd, pos, point);
	}

	const glm::vec3 Camera::screenToWorldPoint(glm::vec3 point) const
	{
		float width = _renderer->getWidth();
		float height = _renderer->getHeight();

		if (renderTexture != nullptr)
		{
			width = renderTexture->getWidth();
			height = renderTexture->getHeight();
		}

		glm::mat4 view = getViewMatrix();
		glm::mat4 proj = getProjectionMatrix();

		return Core::Mathf::screenToWorldPoint(width, height, view, proj, point);
	}
} // namespace Core
