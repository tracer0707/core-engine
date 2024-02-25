#pragma once

#include <algorithm>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Ray.h"
#include "AxisAlignedBox.h"

namespace Core
{
	class Mathf
	{
	public:
		const static float PI;
		const static float fDeg2Rad;
		const static float fRad2Deg;
		const static float POS_INFINITY;
		const static float NEG_INFINITY;

		static float remap(float value, float min1, float max1, float min2, float max2);

		static float clamp01(float value);

		static float clamp(float value, float min1, float max1);

		static float lerp(float a, float b, float t);

		static glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t);

		static float normalize(float input, float min = -1, float max = 1);

		static float randomFloat(float min, float max);

		static int randomInt(int min, int max);

		static float smoothstep(float edge0, float edge1, float x);

		static glm::highp_quat toQuaternion(glm::vec3 value);

		// yaw (Z), pitch (Y), roll (X)
		static glm::highp_quat toQuaternion(double yaw, double pitch, double roll);

		static glm::vec3 toEuler(glm::highp_quat q);

		static glm::vec3 normalizeAngles(glm::vec3 angles);

		static float normalizeAngle(float angle);

		static float squaredLength(glm::vec3 v);

		static void makeCeil(glm::vec3& v, const glm::vec3& cmp);

		static void makeCeil(glm::vec2& v, const glm::vec2& cmp);

		static void makeFloor(glm::vec3& v, const glm::vec3& cmp);

		static void makeFloor(glm::vec2& v, const glm::vec2& cmp);

		static std::pair<bool, float> intersects(const Ray& ray, const glm::vec3& a,
			const glm::vec3& b, const glm::vec3& c, const glm::vec3& normal,
			bool positiveSide, bool negativeSide);
		
		static std::pair<bool, float> intersects(const Ray& ray, const glm::vec3& a,
			const glm::vec3& b, const glm::vec3& c,
			bool positiveSide, bool negativeSide);

		//Intersects Sphere vs AAB
		static bool intersects(glm::vec3 center, float radius, AxisAlignedBox box);
		static bool intersects(AxisAlignedBox a, AxisAlignedBox b);

		static glm::vec3 calculateBasicFaceNormalWithoutNormalize(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);

		static glm::mat3 fromAxes(glm::vec3 right, glm::vec3 up, glm::vec3 dir);

		static bool isOnLine(glm::vec2& point, glm::vec2& start, glm::vec2& end);

		static float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);

		static bool pointInTriangle(glm::vec2 s, glm::vec2 a, glm::vec2 b, glm::vec2 c);

		static glm::vec2 rotateUV(glm::vec2 uv, float rotation);

		static float angleBetweenVectors(glm::vec3 dir1, glm::vec3 dir2);

		static glm::vec3 rotateAround(glm::vec3 aPointToRotate,
			glm::vec3 aRotationCenter,
			glm::mat4x4 aRotation);

		static glm::vec3 getForward(glm::quat rotation);

		static glm::vec3 getUp(glm::quat rotation);

		static glm::vec3 getRight(glm::quat rotation);

		static glm::vec3 barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);
	};
}