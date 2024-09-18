#include "Camera.h"

#include "Transform.h"
#include "ComponentList.h"

#include "../Renderer/Renderer.h"
#include "../Math/Mathf.h"
#include "../Math/Plane.h"
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

    const Ray Camera::getCameraToViewportRay(float screenX, float screenY)
    {
        float width = Renderer::singleton()->getWidth();
        float height = Renderer::singleton()->getHeight();

        glm::mat4x4 mViewProjInverse;
        mViewProjInverse = glm::inverse(getProjectionMatrix() * getViewMatrix());

        float mox = screenX * 2.0f - 1.0f;
        float moy = (1.0f - screenY) * 2.0f - 1.0f;

        glm::vec4 rayOrigin = mViewProjInverse * glm::vec4(mox, moy, 0.0f, 1.0f);
        rayOrigin *= 1.0f / rayOrigin.w;
        glm::vec4 rayEnd = mViewProjInverse * glm::vec4(mox, moy, 1.0f - FLT_EPSILON, 1.0f);
        rayEnd *= 1.0f / rayEnd.w;
        glm::vec4 rayDir = glm::normalize(rayEnd - rayOrigin);

        return Ray(rayOrigin, rayDir);
    }

    const glm::vec3 Camera::worldToScreenPoint(glm::vec3 point)
    {
        Transform* transform = owner->findComponent<Transform*>();
        if (transform == nullptr) return glm::vec3(0.0f);

        float width = Renderer::singleton()->getWidth();
        float height = Renderer::singleton()->getHeight();

        glm::vec4 spPoint = getProjectionMatrix() * (getViewMatrix() * glm::vec4(point, 1.0f));

        bool isInFrustum =
            (spPoint.x < -1.0f) ||
            (spPoint.x > 1.0f) ||
            (spPoint.y < -1.0f) ||
            (spPoint.y > 1.0f);

        Plane cameraPlane = Plane(transform->getForward(), transform->getPosition());

        if (cameraPlane.getSide(point) == Plane::NEGATIVE_SIDE)
            isInFrustum = false;

        glm::vec3 spoint = glm::vec3(spPoint) / spPoint.w;

        glm::vec3 screenSpacePoint = glm::vec3(0);
        screenSpacePoint.x = ((spoint.x * 0.5f) + 0.5f) * width;
        screenSpacePoint.y = height - (((spoint.y * 0.5f) + 0.5f) * height);
        screenSpacePoint.z = isInFrustum ? 1.0f : -1.0f;

        return screenSpacePoint;
    }

    const glm::vec3 Camera::screenToWorldPoint(glm::vec3 point)
    {
        float width = Renderer::singleton()->getWidth();
        float height = Renderer::singleton()->getHeight();

        float scrx = point.x / width;
        float scry = point.y / height;

        Ray ray = getCameraToViewportRay((float)(scrx), (float)(scry));
        glm::vec3 vect = ray.origin + ray.direction * point.z;

        return vect;
    }
}
