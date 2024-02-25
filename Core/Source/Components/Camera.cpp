#include "Camera.h"

#include "../Renderer/Renderer.h"
#include "../Math/Mathf.h"

namespace Core
{
    const glm::mat4x4 Camera::getViewMatrix()
    {
        glm::vec3 right = Mathf::getRight(rotation);
        glm::vec3 forward = Mathf::getForward(rotation);

        glm::vec3 at = position + forward;
        glm::vec3 up = glm::cross(right, forward);

        return glm::lookAt(position, at, up);
    }

    const glm::mat4x4 Camera::getProjectionMatrix(float aspect)
    {
        return glm::perspective(glm::radians(fov), aspect, near, far);
    }
}
