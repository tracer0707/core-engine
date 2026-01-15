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
		private:
			Transformable* parent = nullptr;
			std::vector<Transformable*> children;

			glm::vec3 position{0.0f};
			glm::quat rotation{1, 0, 0, 0};
			glm::vec3 scale{1.0f};

			mutable glm::mat4 cachedWorldMatrix{1.0f};
			mutable bool dirty = true;

		public:
			Transformable();
			~Transformable();

			void setParent(Transformable* value);

			void addChild(Transformable* child);
			void removeChild(Transformable* child);

			void markDirty();

			// ===== WORLD =====
			glm::vec3 getPosition() const;
			glm::quat getRotation() const;
			glm::vec3 getScale() const;

			void setPosition(const glm::vec3& value);
			void setRotation(const glm::quat& value);
			void setScale(const glm::vec3& value);

			// ===== LOCAL =====
			glm::vec3 getLocalPosition() const;
			glm::quat getLocalRotation() const;
			glm::vec3 getLocalScale() const;

			void setLocalPosition(const glm::vec3& value);
			void setLocalRotation(const glm::quat& value);
			void setLocalScale(const glm::vec3& value);

			// matrices
			glm::mat4 getLocalTransformMatrix() const;
			glm::mat4 getTransformMatrix() const;

			// directions (world)
			glm::vec3 getForward() const;
			glm::vec3 getUp() const;
			glm::vec3 getRight() const;

			glm::mat3 getLocalAxes() const;

			// movement
			void yaw(float degree, bool world = false);
			void pitch(float degree, bool world = false);
			void roll(float degree, bool world = false);

			void rotate(const glm::vec3& axis, float degree, bool world = false);
			void rotate(const glm::quat& q, bool world = false);
			void translate(const glm::vec3& direction, bool world = false);

			// conversions
			glm::vec3 worldToLocalPosition(const glm::vec3& worldPos) const;
			glm::quat worldToLocalRotation(const glm::quat& worldRot) const;

			glm::vec3 localToWorldPosition(const glm::vec3& localPos) const;
			glm::quat localToWorldRotation(const glm::quat& localRot) const;
	};
} // namespace Core