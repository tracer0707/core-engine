#pragma once

#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Shared/String.h"

namespace Core
{
	class Object;

	class Transform: public Component
	{
	private:
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::quat rotation = glm::identity<glm::quat>();
		glm::vec3 scale = glm::vec3(1, 1, 1);

		glm::vec3 localPosition = glm::vec3(0, 0, 0);
		glm::quat localRotation = glm::identity<glm::quat>();
		glm::vec3 localScale = glm::vec3(1, 1, 1);

		Transform* parent = nullptr;

	public:
		Transform(Object* owner);
		virtual ~Transform();

		virtual UInt32 getComponentType();

		Transform* getParent() { return parent; }
		void setParent(Transform* value);

		void updateTransform();

		glm::vec3 getPosition() { return position; }
		glm::vec3 getLocalPosition() { return localPosition; }
		glm::highp_quat getRotation() { return rotation; }
		glm::highp_quat getLocalRotation() { return localRotation; }
		glm::vec3 getScale() { return scale; }
		glm::vec3 getLocalScale() { return localScale; }
		glm::vec3 getForward();
		glm::vec3 getUp();
		glm::vec3 getRight();
		glm::mat4x4 getTransformMatrix();
		glm::mat4x4 getTransformMatrixInverse();
		glm::mat4x4 getLocalTransformMatrix();
		glm::mat3x3 getLocalAxes();

		static glm::mat4x4 makeTransformMatrix(glm::vec3 position, glm::highp_quat rotation, glm::vec3 scale);

		void setPosition(glm::vec3 value, bool updateChildren = true);
		void setLocalPosition(glm::vec3 value, bool updateChildren = true);
		void setRotation(glm::highp_quat value, bool updateChildren = true);
		void setLocalRotation(glm::highp_quat value, bool updateChildren = true);
		void setScale(glm::vec3 value, bool updateChildren = true);
		void setLocalScale(glm::vec3 value, bool updateChildren = true);
		void setTransformMatrix(glm::mat4x4 value);
		void setLocalTransformMatrix(glm::mat4x4 value);

		void yaw(float degree, bool world = true);
		void pitch(float degree, bool world = true);
		void roll(float degree, bool world = true);
		void rotate(glm::vec3 axis, float degree, bool world = true);
		void rotate(glm::highp_quat q, bool world = true);
		void translate(glm::vec3 direction, bool world = true);

		glm::vec3 worldToLocalPosition(glm::vec3 worldPos);
		glm::highp_quat worldToLocalRotation(glm::highp_quat worldRot);
		glm::vec3 localToWorldPosition(glm::vec3 localPos);
		glm::highp_quat localToWorldRotation(glm::highp_quat localRot);
	};
}