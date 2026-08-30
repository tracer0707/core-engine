#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "../Shared/String.h"
#include "../Math/Ray.h"

#include "Component.h"

namespace Core
{
    class Object;
    class RenderTexture;
    class Renderer;

    class Camera : public Component
    {
      private:
        Renderer* _renderer = nullptr;
        RenderTexture* renderTexture = nullptr;

        float _fov = 75.0f;
        float _near = 0.01f;
        float _far = 1000.0f;

      public:
        Camera(Object* owner, Renderer* renderer);
        virtual ~Camera();

        virtual ComponentType getComponentType() const { return ComponentType::Camera; }

        float getFov() const { return _fov; }
        void setFov(float value) { _fov = value; }

        float getNear() const { return _near; }
        void setNear(float value) { _near = value; }

        float getFar() const { return _far; }
        void setFar(float value) { _far = value; }

        const glm::mat4 getViewMatrix() const;
        const glm::mat4 getProjectionMatrix() const;

        const Ray getCameraToViewportRay(float x, float y, float offsetX, float offsetY) const;

        const glm::vec3 worldToScreenPoint(glm::vec3 point) const;
        const glm::vec3 screenToWorldPoint(glm::vec3 point, float offsetX, float offsetY) const;

        void setRenderTexture(RenderTexture* value) { renderTexture = value; }
        RenderTexture* getRenderTexture() const { return renderTexture; }
    };
} // namespace Core