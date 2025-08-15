#include "Transformable.h"

#include <glm/gtx/matrix_decompose.hpp>

#include "../Math/Mathf.h"

namespace Core
{
	Transformable::Transformable()
	{
		worldMtx = glm::identity<glm::mat4>();
	}

	void Transformable::setParent(Transformable* value)
	{
		parent = value;
	}

	glm::vec3 Transformable::getPosition()
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(worldMtx, scale, rotation, translation, skew, perspective);

		return translation;
	}

	glm::vec3 Transformable::getLocalPosition()
	{
		if (parent == nullptr) return getPosition();
		return parent->getTransformMatrixInverse() * glm::vec4(getPosition(), 1.0f);
	}

	glm::quat Transformable::getRotation()
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		rotation = glm::conjugate(rotation);

		glm::decompose(worldMtx, scale, rotation, translation, skew, perspective);

		return rotation;
	}

	glm::quat Transformable::getRotationInverse()
	{
		return glm::inverse(getRotation());
	}

	glm::quat Transformable::getLocalRotation()
	{
		if (parent == nullptr) return getRotation();
		return parent->getRotationInverse() * getRotation();
	}

	glm::vec3 Transformable::getScale()
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(worldMtx, scale, rotation, translation, skew, perspective);

		return scale;
	}

	glm::vec3 Transformable::getLocalScale()
	{
		if (parent == nullptr) return getScale();
		return getScale() / parent->getScale();
	}

	void Transformable::setPosition(glm::vec3 value)
	{
		worldMtx = makeTransformMatrix(value, getRotation(), getScale());
	}

	void Transformable::setLocalPosition(glm::vec3 value)
	{
		if (parent != nullptr)
		{
			value = parent->getTransformMatrix() * glm::vec4(value, 1.0f);
		}

		worldMtx = makeTransformMatrix(value, getRotation(), getScale());
	}

	void Transformable::setRotation(glm::quat value)
	{
		worldMtx = makeTransformMatrix(getPosition(), value, getScale());
	}

	void Transformable::setLocalRotation(glm::quat value)
	{
		if (parent != nullptr)
		{
			value = parent->getRotation() * value;
		}

		worldMtx = makeTransformMatrix(getPosition(), value, getScale());
	}

	void Transformable::setScale(glm::vec3 value)
	{
		worldMtx = makeTransformMatrix(getPosition(), getRotation(), value);
	}

	void Transformable::setLocalScale(glm::vec3 value)
	{
		if (parent != nullptr)
		{
			value = parent->getScale() * value;
		}

		worldMtx = makeTransformMatrix(getPosition(), getRotation(), value);
	}

	glm::vec3 Transformable::getForward()
	{
		return glm::normalize(getRotation() * glm::vec3(0, 0, 1));
	}

	glm::vec3 Transformable::getUp()
	{
		return glm::normalize(getRotation() * glm::vec3(0, 1, 0));
	}

	glm::vec3 Transformable::getRight()
	{
		return glm::normalize(getRotation() * glm::vec3(1, 0, 0));
	}

	glm::mat3 Transformable::getLocalAxes()
	{
		glm::vec3 axisX = glm::vec3(1, 0, 0);
		glm::vec3 axisY = glm::vec3(0, 1, 0);
		glm::vec3 axisZ = glm::vec3(0, 0, 1);

		glm::quat lr = getLocalRotation();

		axisX = lr * axisX;
		axisY = lr * axisY;
		axisZ = lr * axisZ;

		return glm::mat3(axisX.x, axisY.x, axisZ.x,
			axisX.y, axisY.y, axisZ.y,
			axisX.z, axisY.z, axisZ.z);
	}

	glm::mat4& Transformable::getTransformMatrix()
	{
		return worldMtx;
	}

	glm::mat4 Transformable::getTransformMatrixInverse()
	{
		return glm::inverse(getTransformMatrix());
	}

	glm::mat4 Transformable::getLocalTransformMatrix()
	{
		if (parent != nullptr)
			return parent->getTransformMatrixInverse() * getTransformMatrix();
		else
			return getTransformMatrix();
	}

	void Transformable::setTransformMatrix(glm::mat4 value)
	{
		worldMtx = value;
	}

	void Transformable::setLocalTransformMatrix(glm::mat4 value)
	{
		if (parent != nullptr)
			setTransformMatrix(parent->getTransformMatrix() * value);
		else
			setTransformMatrix(value);
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

	void Transformable::rotate(glm::vec3 axis, float degree, bool world)
	{
		glm::quat q = glm::angleAxis(degree * Mathf::fDeg2Rad, axis);
		rotate(q, world);
	}

	void Transformable::rotate(glm::quat q, bool world)
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

	void Transformable::translate(glm::vec3 direction, bool world)
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

	glm::vec3 Transformable::worldToLocalPosition(glm::vec3 worldPos)
	{
		return getRotationInverse() * (worldPos - getPosition()) / getScale();
	}

	glm::quat Transformable::worldToLocalRotation(glm::quat worldRot)
	{
		return getRotationInverse() * worldRot;
	}

	glm::vec3 Transformable::localToWorldPosition(glm::vec3 localPos)
	{
		return getTransformMatrix() * glm::vec4(localPos, 1.0f);
	}

	glm::quat Transformable::localToWorldRotation(glm::quat localRot)
	{
		return getRotation() * localRot;
	}

	glm::mat4 Transformable::makeTransformMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	{
		glm::mat4 trans = glm::identity<glm::mat4>();
		glm::mat4 rotMat = glm::mat4_cast(rotation);

		glm::vec3 pos = glm::inverse(rotMat) * (glm::vec4(position, 1.0f));

		trans = glm::translate(trans, pos);
		trans = rotMat * trans;
		trans = glm::scale(trans, scale);

		return trans;
	}
}