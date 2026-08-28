#include "Transform.h"
#include "../Math/Mathf.h"

namespace Core
{
	Transform::Transform() = default;

	Transform::~Transform()
	{
		setParent(nullptr);
	}

	// ================= HIERARCHY =================

	void Transform::setParent(Transform* value)
	{
		if (parent == value) return;

		if (parent) parent->removeChild(this);

		parent = value;

		if (parent) parent->addChild(this);

		markDirty();
	}

	void Transform::addChild(Transform* child)
	{
		if (!child || child == this) return;
		children.push_back(child);
		child->markDirty();
	}

	void Transform::removeChild(Transform* child)
	{
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end()) children.erase(it);
	}

	void Transform::markDirty()
	{
		if (dirty) return;
		dirty = true;

		for (auto* c : children)
			c->markDirty();
	}

	// ================= LOCAL =================

	glm::vec3 Transform::getLocalPosition() const
	{
		return position;
	}
	glm::quat Transform::getLocalRotation() const
	{
		return rotation;
	}
	glm::vec3 Transform::getLocalScale() const
	{
		return scale;
	}

	void Transform::setLocalPosition(const glm::vec3& value)
	{
		position = value;
		markDirty();
	}

	void Transform::setLocalRotation(const glm::quat& value)
	{
		rotation = value;
		markDirty();
	}

	void Transform::setLocalScale(const glm::vec3& value)
	{
		scale = value;
		markDirty();
	}

	// ================= WORLD =================

	glm::vec3 Transform::getPosition() const
	{
		return glm::vec3(getTransformMatrix()[3]);
	}

	glm::quat Transform::getRotation() const
	{
		return parent ? parent->getRotation() * rotation : rotation;
	}

	glm::vec3 Transform::getScale() const
	{
		return parent ? parent->getScale() * scale : scale;
	}

	void Transform::setPosition(const glm::vec3& value)
	{
		if (parent)
		{
			position = glm::inverse(parent->getRotation()) * ((value - parent->getPosition()) / parent->getScale());
		}
		else
			position = value;

		markDirty();
	}

	void Transform::setRotation(const glm::quat& value)
	{
		if (parent)
			rotation = glm::inverse(parent->getRotation()) * value;
		else
			rotation = value;

		markDirty();
	}

	void Transform::setScale(const glm::vec3& value)
	{
		if (parent)
			scale = value / parent->getScale();
		else
			scale = value;

		markDirty();
	}

	// ================= MATRICES =================

	glm::mat4 Transform::getLocalTransformMatrix() const
	{
		return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
	}

	glm::mat4 Transform::getTransformMatrix() const
	{
		if (dirty)
		{
			if (parent)
				cachedWorldMatrix = parent->getTransformMatrix() * getLocalTransformMatrix();
			else
				cachedWorldMatrix = getLocalTransformMatrix();

			dirty = false;
		}
		return cachedWorldMatrix;
	}

	// ================= DIRECTIONS =================

	glm::vec3 Transform::getForward() const
	{
		return glm::normalize(getRotation() * glm::vec3(0, 0, 1));
	}

	glm::vec3 Transform::getUp() const
	{
		return glm::normalize(getRotation() * glm::vec3(0, 1, 0));
	}

	glm::vec3 Transform::getRight() const
	{
		return glm::normalize(getRotation() * glm::vec3(1, 0, 0));
	}

	glm::mat3 Transform::getLocalAxes() const
	{
		return glm::mat3(getRight(), getUp(), getForward());
	}

	// ================= MOVEMENT =================

	void Transform::yaw(float d, bool w)
	{
		rotate({0, 1, 0}, d, w);
	}
	void Transform::pitch(float d, bool w)
	{
		rotate({1, 0, 0}, d, w);
	}
	void Transform::roll(float d, bool w)
	{
		rotate({0, 0, 1}, d, w);
	}

	void Transform::rotate(const glm::vec3& axis, float degree, bool world)
	{
		rotate(glm::angleAxis(glm::radians(degree), glm::normalize(axis)), world);
	}

	void Transform::rotate(const glm::quat& q, bool world)
	{
		if (world)
			setRotation(q * getRotation());
		else
			rotation = rotation * q;

		markDirty();
	}

	void Transform::translate(const glm::vec3& dir, bool world)
	{
		if (world)
			setPosition(getPosition() + dir);
		else
			position += rotation * dir;

		markDirty();
	}

	// ================= CONVERSIONS =================

	glm::vec3 Transform::worldToLocalPosition(const glm::vec3& worldPos) const
	{
		glm::vec3 p = worldPos - getPosition();
		p = glm::inverse(getRotation()) * p;
		p /= getScale();
		return p;
	}

	glm::quat Transform::worldToLocalRotation(const glm::quat& worldRot) const
	{
		return glm::inverse(getRotation()) * worldRot;
	}

	glm::vec3 Transform::localToWorldPosition(const glm::vec3& localPos) const
	{
		glm::vec3 p = localPos * getScale();
		p = getRotation() * p;
		p += getPosition();
		return p;
	}

	glm::quat Transform::localToWorldRotation(const glm::quat& localRot) const
	{
		return getRotation() * localRot;
	}
} // namespace Core