#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
	class Transformable
	{
	protected:
		Transformable* parent = nullptr;

		glm::mat4 worldMtx;

	public:
		Transformable();
		virtual ~Transformable() = default;

		Transformable* getParent() { return parent; }
		void setParent(Transformable* value);

		glm::vec3 getPosition();
		glm::vec3 getLocalPosition();
		glm::quat getRotation();
		glm::quat getRotationInverse();
		glm::quat getLocalRotation();
		glm::vec3 getScale();
		glm::vec3 getLocalScale();
		glm::vec3 getForward();
		glm::vec3 getUp();
		glm::vec3 getRight();

		glm::mat4& getTransformMatrix();
		glm::mat4 getTransformMatrixInverse();
		glm::mat4 getLocalTransformMatrix();
		glm::mat3 getLocalAxes();

		static glm::mat4 makeTransformMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

		void setPosition(glm::vec3 value);
		void setLocalPosition(glm::vec3 value);
		void setRotation(glm::quat value);
		void setLocalRotation(glm::quat value);
		void setScale(glm::vec3 value);
		void setLocalScale(glm::vec3 value);
		void setTransformMatrix(glm::mat4 value);
		void setLocalTransformMatrix(glm::mat4 value);

		void yaw(float degree, bool world = true);
		void pitch(float degree, bool world = true);
		void roll(float degree, bool world = true);
		void rotate(glm::vec3 axis, float degree, bool world = true);
		void rotate(glm::quat q, bool world = true);
		void translate(glm::vec3 direction, bool world = true);

		glm::vec3 worldToLocalPosition(glm::vec3 worldPos);
		glm::quat worldToLocalRotation(glm::quat worldRot);
		glm::vec3 localToWorldPosition(glm::vec3 localPos);
		glm::quat localToWorldRotation(glm::quat localRot);
	};
}