#include "Camera.h"

#include "Transform.h"
#include "ComponentList.h"

#include "../Renderer/Renderer.h"
#include "../Assets/RenderTexture.h"
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

    unsigned int Camera::getComponentType()
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
        int w = _renderer->getWidth();
        int h = _renderer->getHeight();

        if (renderTexture != nullptr)
        {
            w = renderTexture->getWidth();
            h = renderTexture->getHeight();
        }

        float aspect = (float)w / (float)h;

        return glm::perspective(glm::radians(_fov), aspect, _near, _far);
    }

    const Ray Camera::getCameraToViewportRay(float x, float y, float offsetX, float offsetY)
    {
        float screenW = _renderer->getWidth();
        float screenH = _renderer->getHeight();

        if (renderTexture != nullptr)
        {
            screenW = renderTexture->getWidth();
            screenH = renderTexture->getHeight();
        }

        glm::mat4x4 mViewProjInverse;
        mViewProjInverse = glm::inverse(getProjectionMatrix() * getViewMatrix());

        float mx = (x - offsetX);
        float my = (y - offsetY);

        float mox = (mx / screenW) * 2.0f - 1.0f;
        float moy = (1.0f - (my / screenH)) * 2.0f - 1.0f;

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

        float width = _renderer->getWidth();
        float height = _renderer->getHeight();

        if (renderTexture != nullptr)
        {
            width = renderTexture->getWidth();
            height = renderTexture->getHeight();
        }

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

    const glm::vec3 Camera::screenToWorldPoint(glm::vec3 point, float offsetX, float offsetY)
    {
        float width = _renderer->getWidth();
        float height = _renderer->getHeight();

        if (renderTexture != nullptr)
        {
            width = renderTexture->getWidth();
            height = renderTexture->getHeight();
        }

        float scrx = point.x / width;
        float scry = point.y / height;

        Ray ray = getCameraToViewportRay(scrx, scry, offsetX, offsetY);
        glm::vec3 vect = ray.origin + ray.direction * point.z;

        return vect;
    }
}
