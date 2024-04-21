#include "Camera.h"

#include "Transform.h"
#include "../Renderer/Renderer.h"
#include "../Math/Mathf.h"

namespace Core
{
    UString Camera::COMPONENT_TYPE = "Camera";

    Camera::Camera()
    {
        transform = new Transform();
    }

    Camera::~Camera()
    {
        delete transform;
        transform = nullptr;
    }

    UString Camera::getComponentType()
    {
        return COMPONENT_TYPE;
    }

    const glm::mat4x4 Camera::getViewMatrix()
    {
        glm::vec3 right = transform->getRight();
        glm::vec3 forward = transform->getForward();
        glm::vec3 position = transform->getPosition();

        glm::vec3 at = position + forward;
        glm::vec3 up = glm::cross(right, forward);

        return glm::lookAt(position, at, up);
    }

    const glm::mat4x4 Camera::getProjectionMatrix(float aspect)
    {
        return glm::perspective(glm::radians(_fov), aspect, _near, _far);
    }
}
