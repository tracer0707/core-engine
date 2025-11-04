#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "../Shared/List.h"

namespace Core
{
    class Transformable
    {
      protected:
        Transformable* parent = nullptr;
        List<Transformable*> children;

        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        mutable bool dirty = true;
        mutable glm::mat4 cachedLocalMatrix = glm::mat4(1.0f);
        mutable glm::mat4 cachedWorldMatrix = glm::mat4(1.0f);

        void markDirty();

      public:
        Transformable() = default;
        virtual ~Transformable() = default;

        Transformable* getParent() const { return parent; }
        void setParent(Transformable* value);

        void addChild(Transformable* child);
        void removeChild(Transformable* child);
        const List<Transformable*>& getChildren() const { return children; }

        glm::vec3 getPosition() const;
        glm::vec3 getLocalPosition() const;
        glm::quat getRotation() const;
        glm::quat getRotationInverse() const;
        glm::quat getLocalRotation() const;
        glm::vec3 getScale() const;
        glm::vec3 getLocalScale() const;

        glm::vec3 getForward() const;
        glm::vec3 getUp() const;
        glm::vec3 getRight() const;

        glm::mat4 getTransformMatrix() const;
        glm::mat4 getTransformMatrixInverse() const;
        glm::mat4 getLocalTransformMatrix() const;
        glm::mat3 getLocalAxes() const;

        void setPosition(const glm::vec3& value);
        void setLocalPosition(const glm::vec3& value);
        void setRotation(const glm::quat& value);
        void setLocalRotation(const glm::quat& value);
        void setScale(const glm::vec3& value);
        void setLocalScale(const glm::vec3& value);

        void yaw(float degree, bool world = true);
        void pitch(float degree, bool world = true);
        void roll(float degree, bool world = true);
        void rotate(const glm::vec3& axis, float degree, bool world = true);
        void rotate(const glm::quat& q, bool world = true);
        void translate(const glm::vec3& direction, bool world = true);

        glm::vec3 worldToLocalPosition(const glm::vec3& worldPos) const;
        glm::quat worldToLocalRotation(const glm::quat& worldRot) const;
        glm::vec3 localToWorldPosition(const glm::vec3& localPos) const;
        glm::quat localToWorldRotation(const glm::quat& localRot) const;
    };
} // namespace Core