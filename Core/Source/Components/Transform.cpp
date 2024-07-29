#include "Transform.h"

#include <glm/gtx/matrix_decompose.hpp>

#include "../Math/Mathf.h"

#include "ComponentList.h"

namespace Core
{
	Transform::Transform(Object* owner) : Component(owner) {}
	Transform::~Transform() {}

	UInt32 Transform::getComponentType()
	{
		return COMPONENT_TRANSFORM;
	}

	void Transform::setParent(Transform* value)
	{
		if (parent == value)
			return;

		parent = value;

		setPosition(position, false);
		setRotation(rotation, false);
		setScale(scale, false);
	}

	void Transform::setPosition(glm::vec3 value, bool updateChildren)
	{
		position = value;

		if (parent == nullptr)
			localPosition = position;
		else
			localPosition = glm::inverse(parent->rotation) * (position - parent->position) / parent->scale;
	}

	void Transform::setLocalPosition(glm::vec3 value, bool updateChildren)
	{
		localPosition = value;

		updateTransform();
	}

	void Transform::setRotation(glm::highp_quat value, bool updateChildren)
	{
		rotation = value;

		if (parent == nullptr)
			localRotation = rotation;
		else
			localRotation = glm::inverse(parent->rotation) * rotation;
	}

	void Transform::setLocalRotation(glm::highp_quat value, bool updateChildren)
	{
		localRotation = value;

		updateTransform();
	}

	void Transform::setScale(glm::vec3 value, bool updateChildren)
	{
		scale = value;

		if (scale.x == 0) scale.x = 0.0001f;
		if (scale.y == 0) scale.y = 0.0001f;
		if (scale.z == 0) scale.z = 0.0001f;

		if (parent == nullptr)
			localScale = scale;
		else
			localScale = scale / parent->getScale();
	}

	void Transform::setLocalScale(glm::vec3 value, bool updateChildren)
	{
		localScale = value;

		if (localScale.x == 0) localScale.x = 0.0001f;
		if (localScale.y == 0) localScale.y = 0.0001f;
		if (localScale.z == 0) localScale.z = 0.0001f;

		updateTransform();
	}

	glm::vec3 Transform::getForward()
	{
		return glm::normalize(rotation * glm::vec3(0, 0, 1));
	}

	glm::vec3 Transform::getUp()
	{
		return glm::normalize(rotation * glm::vec3(0, 1, 0));
	}

	glm::vec3 Transform::getRight()
	{
		return glm::normalize(rotation * glm::vec3(1, 0, 0));
	}

	glm::mat3x3 Transform::getLocalAxes()
	{
		glm::vec3 axisX = glm::vec3(1, 0, 0);
		glm::vec3 axisY = glm::vec3(0, 1, 0);
		glm::vec3 axisZ = glm::vec3(0, 0, 1);

		axisX = getLocalRotation() * axisX;
		axisY = getLocalRotation() * axisY;
		axisZ = getLocalRotation() * axisZ;

		return glm::mat3x3(axisX.x, axisY.x, axisZ.x,
			axisX.y, axisY.y, axisZ.y,
			axisX.z, axisY.z, axisZ.z);
	}

	glm::mat4x4 Transform::getTransformMatrix()
	{
		glm::mat4x4 trans = glm::identity<glm::mat4x4>();
		glm::mat4x4 rotMat = glm::mat4_cast(rotation);

		glm::vec3 pos = glm::inverse(rotMat) * (glm::vec4(position, 1.0f));

		trans = glm::translate(trans, pos);
		trans = rotMat * trans;
		trans = glm::scale(trans, scale);

		return trans;
	}

	glm::mat4x4 Transform::getTransformMatrixInverse()
	{
		return glm::inverse(getTransformMatrix());
	}

	glm::mat4x4 Transform::getLocalTransformMatrix()
	{
		if (parent != nullptr)
			return glm::inverse(parent->getTransformMatrix()) * getTransformMatrix();
		else
			return getTransformMatrix();
	}

	void Transform::setTransformMatrix(glm::mat4x4 value)
	{
		glm::vec3 pos = glm::vec3(0, 0, 0);
		glm::vec3 scl = glm::vec3(1, 1, 1);
		glm::highp_quat rot = glm::identity<glm::highp_quat>();

		glm::vec3 scew = glm::vec3(0, 0, 0);
		glm::vec4 persp = glm::vec4(0, 0, 0, 0);

		glm::decompose(value, scl, rot, pos, scew, persp);

		setPosition(pos, false);
		setRotation(rot, false);
		setScale(scl, false);
	}

	void Transform::setLocalTransformMatrix(glm::mat4x4 value)
	{
		if (parent != nullptr)
			setTransformMatrix(parent->getTransformMatrix() * value);
		else
			setTransformMatrix(value);
	}

	void Transform::yaw(float degree, bool world)
	{
		rotate(glm::vec3(0, 1, 0), degree, world);
	}

	void Transform::pitch(float degree, bool world)
	{
		rotate(glm::vec3(1, 0, 0), degree, world);
	}

	void Transform::roll(float degree, bool world)
	{
		rotate(glm::vec3(0, 0, 1), degree, world);
	}

	void Transform::rotate(glm::vec3 axis, float degree, bool world)
	{
		glm::highp_quat q = glm::angleAxis(degree * Mathf::fDeg2Rad, axis);
		rotate(q, world);
	}

	void Transform::rotate(glm::highp_quat q, bool world)
	{
		if (world)
		{
			setLocalRotation(getLocalRotation() * glm::inverse(getRotation()) * q * getRotation());
		}
		else
		{
			setLocalRotation(getLocalRotation() * q);
		}
	}

	void Transform::translate(glm::vec3 direction, bool world)
	{
		if (world)
		{
			setPosition(getPosition() + getRotation() * direction);
		}
		else
		{
			if (parent != nullptr)
				setLocalPosition(getLocalPosition() + direction);
			else
				setPosition(getPosition() + getRotation() * direction);
		}
	}

	glm::vec3 Transform::worldToLocalPosition(glm::vec3 worldPos)
	{
		return glm::inverse(rotation) * (worldPos - position) / scale;
	}

	glm::highp_quat Transform::worldToLocalRotation(glm::highp_quat worldRot)
	{
		return glm::inverse(rotation) * worldRot;
	}

	glm::vec3 Transform::localToWorldPosition(glm::vec3 localPos)
	{
		return getTransformMatrix() * glm::vec4(localPos, 1.0f);
	}

	glm::highp_quat Transform::localToWorldRotation(glm::highp_quat localRot)
	{
		return rotation * localRot;
	}

	glm::mat4x4 Transform::makeTransformMatrix(glm::vec3 position, glm::highp_quat rotation, glm::vec3 scale)
	{
		glm::mat4x4 trans = glm::identity<glm::mat4x4>();
		glm::mat4x4 rotMat = glm::mat4_cast(rotation);

		glm::vec3 pos = glm::inverse(rotMat) * (glm::vec4(position, 1.0f));

		trans = glm::translate(trans, pos);
		trans = rotMat * trans;
		trans = glm::scale(trans, scale);

		return trans;
	}

	void Transform::updateTransform()
	{
		if (parent != nullptr)
		{
			glm::highp_quat parentOrientation = parent->getRotation();
			rotation = parentOrientation * localRotation;
			glm::vec3 parentScale = parent->getScale();
			scale = parentScale * localScale;
			position = parentOrientation * (parentScale * localPosition);
			position += parent->getPosition();
		}
		else
		{
			position = localPosition;
			rotation = localRotation;
			scale = localScale;
		}
	}
}