#include "Camera.h"

#include "Transform.h"
#include "ComponentList.h"

#include "../Renderer/Renderer.h"
#include "../Math/Mathf.h"
#include "../Scene/Object.h"

namespace Core
{
    Camera::Camera(Object* owner) : Component(owner) {}
    Camera::~Camera() {}

    UInt32 Camera::getComponentType()
    {
        return COMPONENT_CAMERA;
    }

    const glm::mat4 Camera::getViewMatrix()
    {
        Transform* transform = owner->findComponent<Transform*>();

        if (transform == nullptr)
            return glm::identity<glm::mat4>();

        glm::vec3 right = transform->getRight();
        glm::vec3 forward = transform->getForward();
        glm::vec3 position = transform->getPosition();

        glm::vec3 at = position + forward;
        glm::vec3 up = glm::cross(right, forward);

        return glm::lookAt(position, at, up);
    }

    const glm::mat4 Camera::getProjectionMatrix()
    {
        int w = Renderer::singleton()->getWidth();
        int h = Renderer::singleton()->getHeight();

        float aspect = (float)w / (float)h;

        return glm::perspective(glm::radians(_fov), aspect, _near, _far);
    }
}
