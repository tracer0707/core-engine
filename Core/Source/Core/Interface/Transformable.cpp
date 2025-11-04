#include "Transformable.h"
#include "../Math/Mathf.h"

namespace Core
{
    void Transformable::addChild(Transformable* child)
    {
        if (child == nullptr || child == this) return;

        if (child->parent != nullptr) child->parent->removeChild(child);

        child->parent = this;
        children.add(child);
        child->markDirty();
    }

    void Transformable::removeChild(Transformable* child)
    {
        auto it = std::find(children.begin(), children.end(), child);
        if (it != children.end())
        {
            (*it)->parent = nullptr;
            children.remove(*it);
        }
    }

    void Transformable::setParent(Transformable* value)
    {
        if (parent == value) return;

        if (parent != nullptr) parent->removeChild(this);

        parent = value;

        if (parent != nullptr) parent->addChild(this);

        markDirty();
    }

    void Transformable::markDirty()
    {
        if (dirty) return;
        dirty = true;

        for (auto* child : children)
        {
            if (child != nullptr) child->markDirty();
        }
    }

    glm::vec3 Transformable::getPosition() const
    {
        return position;
    }
    glm::quat Transformable::getRotation() const
    {
        return rotation;
    }
    glm::quat Transformable::getRotationInverse() const
    {
        return glm::inverse(rotation);
    }
    glm::vec3 Transformable::getScale() const
    {
        return scale;
    }

    glm::vec3 Transformable::getLocalPosition() const
    {
        if (parent == nullptr) return position;
        glm::vec4 local = glm::inverse(parent->getTransformMatrix()) * glm::vec4(position, 1.0f);
        return glm::vec3(local);
    }

    glm::quat Transformable::getLocalRotation() const
    {
        if (parent == nullptr) return rotation;
        return glm::inverse(parent->getRotation()) * rotation;
    }

    glm::vec3 Transformable::getLocalScale() const
    {
        if (parent == nullptr) return scale;
        return scale / parent->getScale();
    }

    void Transformable::setPosition(const glm::vec3& value)
    {
        position = value;
        markDirty();
    }

    void Transformable::setLocalPosition(const glm::vec3& value)
    {
        if (parent != nullptr)
        {
            glm::vec4 world = parent->getTransformMatrix() * glm::vec4(value, 1.0f);
            position = glm::vec3(world);
        }
        else
            position = value;
        markDirty();
    }

    void Transformable::setRotation(const glm::quat& value)
    {
        rotation = value;
        markDirty();
    }

    void Transformable::setLocalRotation(const glm::quat& value)
    {
        if (parent != nullptr)
            rotation = parent->getRotation() * value;
        else
            rotation = value;
        markDirty();
    }

    void Transformable::setScale(const glm::vec3& value)
    {
        scale = value;
        markDirty();
    }

    void Transformable::setLocalScale(const glm::vec3& value)
    {
        if (parent != nullptr)
            scale = parent->getScale() * value;
        else
            scale = value;
        markDirty();
    }

    glm::mat4 Transformable::getLocalTransformMatrix() const
    {
        if (dirty)
        {
            cachedLocalMatrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
            dirty = false;
        }
        return cachedLocalMatrix;
    }

    glm::mat4 Transformable::getTransformMatrix() const
    {
        if (parent != nullptr)
            cachedWorldMatrix = parent->getTransformMatrix() * getLocalTransformMatrix();
        else
            cachedWorldMatrix = getLocalTransformMatrix();

        return cachedWorldMatrix;
    }

    glm::mat4 Transformable::getTransformMatrixInverse() const
    {
        return glm::inverse(getTransformMatrix());
    }

    glm::vec3 Transformable::getForward() const
    {
        return glm::normalize(rotation * glm::vec3(0, 0, 1));
    }
    glm::vec3 Transformable::getUp() const
    {
        return glm::normalize(rotation * glm::vec3(0, 1, 0));
    }
    glm::vec3 Transformable::getRight() const
    {
        return glm::normalize(rotation * glm::vec3(1, 0, 0));
    }

    glm::mat3 Transformable::getLocalAxes() const
    {
        glm::vec3 x = rotation * glm::vec3(1, 0, 0);
        glm::vec3 y = rotation * glm::vec3(0, 1, 0);
        glm::vec3 z = rotation * glm::vec3(0, 0, 1);
        return glm::mat3(x, y, z);
    }

    void Transformable::yaw(float degree, bool world)
    {
        rotate(glm::vec3(0, 1, 0), degree, world);
    }
    void Transformable::pitch(float degree, bool world)
    {
        rotate(glm::vec3(1, 0, 0), degree, world);
    }
    void Transformable::roll(float degree, bool world)
    {
        rotate(glm::vec3(0, 0, 1), degree, world);
    }

    void Transformable::rotate(const glm::vec3& axis, float degree, bool world)
    {
        glm::quat q = glm::angleAxis(glm::radians(degree), glm::normalize(axis));
        rotate(q, world);
    }

    void Transformable::rotate(const glm::quat& q, bool world)
    {
        if (world)
            rotation = q * rotation;
        else
            rotation = rotation * q;
        markDirty();
    }

    void Transformable::translate(const glm::vec3& direction, bool world)
    {
        if (world)
            position += direction;
        else
            position += rotation * direction;
        markDirty();
    }

    glm::vec3 Transformable::worldToLocalPosition(const glm::vec3& worldPos) const
    {
        glm::vec4 local = getTransformMatrixInverse() * glm::vec4(worldPos, 1.0f);
        return glm::vec3(local);
    }

    glm::quat Transformable::worldToLocalRotation(const glm::quat& worldRot) const
    {
        return glm::inverse(rotation) * worldRot;
    }

    glm::vec3 Transformable::localToWorldPosition(const glm::vec3& localPos) const
    {
        glm::vec4 world = getTransformMatrix() * glm::vec4(localPos, 1.0f);
        return glm::vec3(world);
    }

    glm::quat Transformable::localToWorldRotation(const glm::quat& localRot) const
    {
        return rotation * localRot;
    }
} // namespace Core