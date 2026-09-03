#include "Transform.h"
#include "../Math/Mathf.h"

namespace Core
{
	Transform::Transform(void* owner)
	{
		_owner = owner;
	}

	Transform::~Transform()
	{
		setParent(nullptr);
		_owner = nullptr;
	}

	// ================= HIERARCHY =================

	void Transform::setParent(Transform* value)
	{
		if (_parent == value) return;

		if (_parent != nullptr) _parent->removeChild(this);

		_parent = value;

		if (_parent != nullptr) _parent->addChild(this);

		markDirty();
	}

	void Transform::addChild(Transform* child)
	{
		if (child == nullptr || child == this) return;
		_children.push_back(child);
		child->markDirty();
	}

	void Transform::removeChild(Transform* child)
	{
		auto it = std::find(_children.begin(), _children.end(), child);
		if (it != _children.end()) _children.erase(it);
	}

	void Transform::markDirty()
	{
		if (_dirty) return;
		_dirty = true;

		for (auto* c : _children)
		{
			c->markDirty();
		}
	}

	// ================= LOCAL =================

	glm::vec3 Transform::getLocalPosition() const
	{
		return _position;
	}

	glm::quat Transform::getLocalRotation() const
	{
		return _rotation;
	}

	glm::vec3 Transform::getLocalScale() const
	{
		return _scale;
	}

	void Transform::setLocalPosition(const glm::vec3& value)
	{
		_position = value;
		markDirty();
	}

	void Transform::setLocalRotation(const glm::quat& value)
	{
		_rotation = value;
		markDirty();
	}

	void Transform::setLocalScale(const glm::vec3& value)
	{
		_scale = value;
		markDirty();
	}

	// ================= WORLD =================

	glm::vec3 Transform::getPosition() const
	{
		return glm::vec3(getTransformMatrix()[3]);
	}

	glm::quat Transform::getRotation() const
	{
		return _parent ? _parent->getRotation() * _rotation : _rotation;
	}

	glm::vec3 Transform::getScale() const
	{
		return _parent ? _parent->getScale() * _scale : _scale;
	}

	void Transform::setPosition(const glm::vec3& value)
	{
		if (_parent)
		{
			_position = glm::inverse(_parent->getRotation()) * ((value - _parent->getPosition()) / _parent->getScale());
		}
		else
		{
			_position = value;
		}

		markDirty();
	}

	void Transform::setRotation(const glm::quat& value)
	{
		if (_parent)
		{
			_rotation = glm::inverse(_parent->getRotation()) * value;
		}
		else
		{
			_rotation = value;
		}

		markDirty();
	}

	void Transform::setScale(const glm::vec3& value)
	{
		if (_parent)
		{
			_scale = value / _parent->getScale();
		}
		else
		{
			_scale = value;
		}

		markDirty();
	}

	// ================= MATRICES =================

	glm::mat4 Transform::getLocalTransformMatrix() const
	{
		return glm::translate(glm::mat4(1.0f), _position) * glm::mat4_cast(_rotation) * glm::scale(glm::mat4(1.0f), _scale);
	}

	glm::mat4 Transform::getTransformMatrix() const
	{
		if (_dirty)
		{
			if (_parent)
			{
				_cachedWorldMatrix = _parent->getTransformMatrix() * getLocalTransformMatrix();
			}
			else
			{
				_cachedWorldMatrix = getLocalTransformMatrix();
			}

			_dirty = false;
		}
		return _cachedWorldMatrix;
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
		{
			setRotation(q * getRotation());
		}
		else
		{
			_rotation = _rotation * q;
		}

		markDirty();
	}

	void Transform::translate(const glm::vec3& dir, bool world)
	{
		if (world)
		{
			setPosition(getPosition() + dir);
		}
		else
		{
			_position += _rotation * dir;
		}

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