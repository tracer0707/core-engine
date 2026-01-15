#include "Transformable.h"
#include "../Math/Mathf.h"

namespace Core
{
	Transformable::Transformable() = default;

	Transformable::~Transformable()
	{
		setParent(nullptr);
	}

	// ================= HIERARCHY =================

	void Transformable::setParent(Transformable* value)
	{
		if (parent == value) return;

		if (parent) parent->removeChild(this);

		parent = value;

		if (parent) parent->addChild(this);

		markDirty();
	}

	void Transformable::addChild(Transformable* child)
	{
		if (!child || child == this) return;
		children.push_back(child);
		child->markDirty();
	}

	void Transformable::removeChild(Transformable* child)
	{
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end()) children.erase(it);
	}

	void Transformable::markDirty()
	{
		if (dirty) return;
		dirty = true;

		for (auto* c : children)
			c->markDirty();
	}

	// ================= LOCAL =================

	glm::vec3 Transformable::getLocalPosition() const
	{
		return position;
	}
	glm::quat Transformable::getLocalRotation() const
	{
		return rotation;
	}
	glm::vec3 Transformable::getLocalScale() const
	{
		return scale;
	}

	void Transformable::setLocalPosition(const glm::vec3& value)
	{
		position = value;
		markDirty();
	}

	void Transformable::setLocalRotation(const glm::quat& value)
	{
		rotation = value;
		markDirty();
	}

	void Transformable::setLocalScale(const glm::vec3& value)
	{
		scale = value;
		markDirty();
	}

	// ================= WORLD =================

	glm::vec3 Transformable::getPosition() const
	{
		return glm::vec3(getTransformMatrix()[3]);
	}

	glm::quat Transformable::getRotation() const
	{
		return parent ? parent->getRotation() * rotation : rotation;
	}

	glm::vec3 Transformable::getScale() const
	{
		return parent ? parent->getScale() * scale : scale;
	}

	void Transformable::setPosition(const glm::vec3& value)
	{
		if (parent)
		{
			position = glm::inverse(parent->getRotation()) * ((value - parent->getPosition()) / parent->getScale());
		}
		else
			position = value;

		markDirty();
	}

	void Transformable::setRotation(const glm::quat& value)
	{
		if (parent)
			rotation = glm::inverse(parent->getRotation()) * value;
		else
			rotation = value;

		markDirty();
	}

	void Transformable::setScale(const glm::vec3& value)
	{
		if (parent)
			scale = value / parent->getScale();
		else
			scale = value;

		markDirty();
	}

	// ================= MATRICES =================

	glm::mat4 Transformable::getLocalTransformMatrix() const
	{
		return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
	}

	glm::mat4 Transformable::getTransformMatrix() const
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

	glm::vec3 Transformable::getForward() const
	{
		return glm::normalize(getRotation() * glm::vec3(0, 0, 1));
	}

	glm::vec3 Transformable::getUp() const
	{
		return glm::normalize(getRotation() * glm::vec3(0, 1, 0));
	}

	glm::vec3 Transformable::getRight() const
	{
		return glm::normalize(getRotation() * glm::vec3(1, 0, 0));
	}

	glm::mat3 Transformable::getLocalAxes() const
	{
		return glm::mat3(getRight(), getUp(), getForward());
	}

	// ================= MOVEMENT =================

	void Transformable::yaw(float d, bool w)
	{
		rotate({0, 1, 0}, d, w);
	}
	void Transformable::pitch(float d, bool w)
	{
		rotate({1, 0, 0}, d, w);
	}
	void Transformable::roll(float d, bool w)
	{
		rotate({0, 0, 1}, d, w);
	}

	void Transformable::rotate(const glm::vec3& axis, float degree, bool world)
	{
		rotate(glm::angleAxis(glm::radians(degree), glm::normalize(axis)), world);
	}

	void Transformable::rotate(const glm::quat& q, bool world)
	{
		if (world)
			setRotation(q * getRotation());
		else
			rotation = rotation * q;

		markDirty();
	}

	void Transformable::translate(const glm::vec3& dir, bool world)
	{
		if (world)
			setPosition(getPosition() + dir);
		else
			position += rotation * dir;

		markDirty();
	}

	// ================= CONVERSIONS =================

	glm::vec3 Transformable::worldToLocalPosition(const glm::vec3& worldPos) const
	{
		glm::vec3 p = worldPos - getPosition();
		p = glm::inverse(getRotation()) * p;
		p /= getScale();
		return p;
	}

	glm::quat Transformable::worldToLocalRotation(const glm::quat& worldRot) const
	{
		return glm::inverse(getRotation()) * worldRot;
	}

	glm::vec3 Transformable::localToWorldPosition(const glm::vec3& localPos) const
	{
		glm::vec3 p = localPos * getScale();
		p = getRotation() * p;
		p += getPosition();
		return p;
	}

	glm::quat Transformable::localToWorldRotation(const glm::quat& localRot) const
	{
		return getRotation() * localRot;
	}
} // namespace Core