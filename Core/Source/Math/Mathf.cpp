#pragma once

#include <ctime>

#include "Mathf.h"
#include "Ray.h"

namespace Core
{
	const float Mathf::PI = 4.0f * atan(1.0f);
	const float Mathf::fDeg2Rad = PI / 180.0f;
	const float Mathf::fRad2Deg = 180.0f / PI;
	const float Mathf::POS_INFINITY = std::numeric_limits<float>::infinity();
	const float Mathf::NEG_INFINITY = -std::numeric_limits<float>::infinity();

	float Mathf::remap(float value, float min1, float max1, float min2, float max2)
	{
		float start1 = min1;
		float stop1 = max1;
		float start2 = min2;
		float stop2 = max2;

		if (stop1 < start1) stop1 = start1;
		if (stop2 < start2) stop2 = start2;

		float outgoing = start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));

		return outgoing;
	}

	float Mathf::clamp01(float value)
	{
		if (value < 0.0f)
		{
			return 0.0f;
		}
		if (value > 1.0f)
		{
			return 1.0f;
		}
		return value;
	}

	float Mathf::clamp(float value, float min1, float max1)
	{
		if (value < min1)
			return min1;

		if (value > max1)
			return max1;

		return value;
	}

	float Mathf::lerp(float a, float b, float t)
	{
		return (1.0 - t) * a + t * b;
	}

	glm::vec3 Mathf::lerp(glm::vec3 a, glm::vec3 b, float t)
	{
		t = clamp01(t);
		return glm::vec3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
	}

	float Mathf::normalize(float input, float min, float max)
	{
		float average = (min + max) / 2;
		float range = (max - min) / 2;
		float normalized_x = (input - average) / range;
		return normalized_x;
	}

	float Mathf::randomFloat(float min, float max)
	{
		float random = ((float)rand()) / (float)RAND_MAX;

		float range = max - min;
		return (random * range) + min;
	}

	int Mathf::randomInt(int min, int max)
	{
		int random = min + (rand() % static_cast<int>(max - min + 1));
		return random;
	}

	float Mathf::smoothstep(float edge0, float edge1, float x)
	{
		float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
		return t * t * (3.0 - 2.0 * t);
	}

	glm::highp_quat Mathf::toQuaternion(glm::vec3 value)
	{
		return toQuaternion(value.z, value.y, value.x);
	}

	glm::highp_quat Mathf::toQuaternion(double yaw, double pitch, double roll)
	{
		yaw *= fDeg2Rad;
		pitch *= fDeg2Rad;
		roll *= fDeg2Rad;

		double cy = cos(yaw * 0.5);
		double sy = sin(yaw * 0.5);
		double cp = cos(pitch * 0.5);
		double sp = sin(pitch * 0.5);
		double cr = cos(roll * 0.5);
		double sr = sin(roll * 0.5);

		glm::highp_quat q;
		q.w = cy * cp * cr + sy * sp * sr;
		q.x = cy * cp * sr - sy * sp * cr;
		q.y = sy * cp * sr + cy * sp * cr;
		q.z = sy * cp * cr - cy * sp * sr;

		return q;
	}

	glm::vec3 Mathf::toEuler(glm::highp_quat q)
	{
		glm::vec3 angles;

		float sqw = q.w * q.w;
		float sqx = q.x * q.x;
		float sqy = q.y * q.y;
		float sqz = q.z * q.z;
		float unit = sqx + sqy + sqz + sqw;
		float test = q.x * q.w - q.y * q.z;

		if (test > 0.4995f * unit)
		{
			angles.y = 2.0f * atan2(q.y, q.x);
			angles.x = PI / 2.0f;
			angles.z = 0;
			return normalizeAngles(angles * fRad2Deg);
		}

		if (test < -0.4995f * unit)
		{
			angles.y = -2.0f * atan2(q.y, q.x);
			angles.x = -PI / 2.0f;
			angles.z = 0;
			return normalizeAngles(angles * fRad2Deg);
		}

		// roll (x-axis rotation)
		double sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
		angles.x = std::atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = 2.0 * (q.w * q.y - q.z * q.x);
		if (std::abs(sinp) >= 1.0)
			angles.y = std::copysign(PI / 2.0, sinp);
		else
			angles.y = std::asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = 2.0 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
		angles.z = std::atan2(siny_cosp, cosy_cosp);

		return normalizeAngles(angles * fRad2Deg);
	}

	glm::vec3 Mathf::normalizeAngles(glm::vec3 angles)
	{
		angles.x = normalizeAngle(angles.x);
		angles.y = normalizeAngle(angles.y);
		angles.z = normalizeAngle(angles.z);
		return angles;
	}

	float Mathf::normalizeAngle(float angle)
	{
		while (angle > 360.0f)
			angle -= 360.0f;
		while (angle < 0.0f)
			angle += 360.0f;
		return angle;
	}

	float Mathf::squaredLength(glm::vec3 v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	void Mathf::makeCeil(glm::vec3& v, const glm::vec3& cmp)
	{
		if (cmp.x > v.x) v.x = cmp.x;
		if (cmp.y > v.y) v.y = cmp.y;
		if (cmp.z > v.z) v.z = cmp.z;
	}

	void Mathf::makeCeil(glm::vec2& v, const glm::vec2& cmp)
	{
		if (cmp.x > v.x) v.x = cmp.x;
		if (cmp.y > v.y) v.y = cmp.y;
	}

	void Mathf::makeFloor(glm::vec3& v, const glm::vec3& cmp)
	{
		if (cmp.x < v.x) v.x = cmp.x;
		if (cmp.y < v.y) v.y = cmp.y;
		if (cmp.z < v.z) v.z = cmp.z;
	}

	void Mathf::makeFloor(glm::vec2& v, const glm::vec2& cmp)
	{
		if (cmp.x < v.x) v.x = cmp.x;
		if (cmp.y < v.y) v.y = cmp.y;
	}

	std::pair<bool, float> Mathf::intersects(const Ray& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& normal, bool positiveSide, bool negativeSide)
	{
		float t;
		{
			float denom = glm::dot(normal, ray.direction);

			// Check intersect side
			if (denom > +std::numeric_limits<float>::epsilon())
			{
				if (!negativeSide)
					return std::pair<bool, float>(false, (float)0);
			}
			else if (denom < -std::numeric_limits<float>::epsilon())
			{
				if (!positiveSide)
					return std::pair<bool, float>(false, (float)0);
			}
			else
			{
				// Parallel or triangle area is close to zero when
				// the plane normal not normalised.
				return std::pair<bool, float>(false, (float)0);
			}

			t = glm::dot(normal, a - ray.origin) / denom;

			if (t < 0)
			{
				// Intersection is behind origin
				return std::pair<bool, float>(false, (float)0);
			}
		}

		// Calculate the largest area projection plane in X, Y or Z.
		size_t i0, i1;
		{
			float n0 = abs(normal[0]);
			float n1 = abs(normal[1]);
			float n2 = abs(normal[2]);

			i0 = 1; i1 = 2;
			if (n1 > n2)
			{
				if (n1 > n0) i0 = 0;
			}
			else
			{
				if (n2 > n0) i1 = 0;
			}
		}

		// Check the intersection point is inside the triangle.
		float u1 = b[i0] - a[i0];
		float v1 = b[i1] - a[i1];
		float u2 = c[i0] - a[i0];
		float v2 = c[i1] - a[i1];
		float u0 = t * ray.direction[i0] + ray.origin[i0] - a[i0];
		float v0 = t * ray.direction[i1] + ray.origin[i1] - a[i1];

		float alpha = u0 * v2 - u2 * v0;
		float beta = u1 * v0 - u0 * v1;
		float area = u1 * v2 - u2 * v1;

		// epsilon to avoid float precision error
		const float EPSILON = 1e-6f;

		float tolerance = -EPSILON * area;

		if (area > 0)
		{
			if (alpha < tolerance || beta < tolerance || alpha + beta > area - tolerance)
				return std::pair<bool, float>(false, (float)0);
		}
		else
		{
			if (alpha > tolerance || beta > tolerance || alpha + beta < area - tolerance)
				return std::pair<bool, float>(false, (float)0);
		}

		return std::pair<bool, float>(true, (float)t);
	}

	std::pair<bool, float> Mathf::intersects(const Ray& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, bool positiveSide, bool negativeSide)
	{
		glm::vec3 normal = calculateBasicFaceNormalWithoutNormalize(a, b, c);
		return intersects(ray, a, b, c, normal, positiveSide, negativeSide);
	}

	bool Mathf::intersects(glm::vec3 center, float radius, AxisAlignedBox box)
	{
		if (box.isNull()) return false;
		if (box.isInfinite()) return true;

		// Use splitting planes
		glm::vec3& min = box.getMinimum();
		glm::vec3& max = box.getMaximum();

		// Arvo's algorithm
		float s, d = 0;
		for (int i = 0; i < 3; ++i)
		{
			if (center[i] < min[i])
			{
				s = center[i] - min[i];
				d += s * s;
			}
			else if (center[i] > max[i])
			{
				s = center[i] - max[i];
				d += s * s;
			}
		}
		return d <= radius * radius;
	}

	bool Mathf::intersects(AxisAlignedBox a, AxisAlignedBox b)
	{
		// SIMD optimized AABB-AABB test
		// Optimized by removing conditional branches
		bool x = std::abs(a.getCenter()[0] - b.getCenter()[0]) <= (a.getHalfSize()[0] + b.getHalfSize()[0]);
		bool y = std::abs(a.getCenter()[1] - b.getCenter()[1]) <= (a.getHalfSize()[1] + b.getHalfSize()[1]);
		bool z = std::abs(a.getCenter()[2] - b.getCenter()[2]) <= (a.getHalfSize()[2] + b.getHalfSize()[2]);

		return x && y && z;
	}

	glm::vec3 Mathf::calculateBasicFaceNormalWithoutNormalize(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
	{
		glm::vec3 normal = glm::cross(v2 - v1, v3 - v1);
		return normal;
	}

	void setColumn(glm::mat3 &mtx, size_t iCol, glm::vec3 vec)
	{
		assert(iCol < 3);
		mtx[0][iCol] = vec.x;
		mtx[1][iCol] = vec.y;
		mtx[2][iCol] = vec.z;
	}

	glm::mat3 Mathf::fromAxes(glm::vec3 right, glm::vec3 up, glm::vec3 dir)
	{
		glm::mat3 mtx = glm::identity<glm::mat3>();

		setColumn(mtx, 0, right);
		setColumn(mtx, 1, up);
		setColumn(mtx, 2, dir);

		return mtx;
	}

	float Mathf::sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
	{
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

	bool Mathf::isOnLine(glm::vec2& point, glm::vec2& start, glm::vec2& end)
	{
		return (((point.x - start.x) / (end.x - start.x)) == ((point.y - start.y) / (end.y - start.y)));
	}

	bool Mathf::pointInTriangle(glm::vec2 s, glm::vec2 a, glm::vec2 b, glm::vec2 c)
	{
		float dX = s.x - c.x;
		float dY = s.y - c.y;
		float dX21 = c.x - b.x;
		float dY12 = b.y - c.y;
		float D = dY12 * (a.x - c.x) + dX21 * (a.y - c.y);
		float p = dY12 * dX + dX21 * dY;
		float t = (c.y - a.y) * dX + (a.x - c.x) * dY;
		if (D < 0) return p <= 0 && t <= 0 && p + t >= D;
		return p >= 0 && t >= 0 && p + t <= D;
	}

	glm::vec2 Mathf::rotateUV(glm::vec2 uv, float rotation)
	{
		float mid = 0.5f;
		return glm::vec2(
			cos(rotation) * (uv.x - mid) + sin(rotation) * (uv.y - mid) + mid,
			cos(rotation) * (uv.y - mid) - sin(rotation) * (uv.x - mid) + mid);
	}

	float Mathf::angleBetweenVectors(glm::vec3 dir1, glm::vec3 dir2)
	{
		float sqrMag1 = dir1.x * dir1.x + dir1.y * dir1.y + dir1.z * dir1.z;
		float sqrMag2 = dir2.x * dir2.x + dir2.y * dir2.y + dir2.z * dir2.z;

		float num = std::sqrt(sqrMag1 * sqrMag2);
		if (num < 1E-15f)
		{
			return 0.0f;
		}

		float f = Mathf::clamp(glm::dot(dir1, dir2) / num, -1.0f, 1.0f);
		return std::acos(f) * 57.29578f;
	}

	glm::vec3 Mathf::rotateAround(glm::vec3 aPointToRotate, glm::vec3 aRotationCenter, glm::mat4x4 aRotation)
	{
		glm::mat4x4 translate =
			glm::translate(
				glm::identity<glm::mat4x4>(),
				aRotationCenter);

		glm::mat4x4 invTranslate = glm::inverse(translate);

		// 1) Translate the object to the center
		// 2) Make the rotation
		// 3) Translate the object back to its original location

		glm::mat4x4 transform = translate * aRotation * invTranslate;

		return transform * glm::vec4(aPointToRotate, 1.0f);
	}

	glm::vec3 Mathf::getForward(glm::quat rotation)
	{
		return glm::normalize(rotation * glm::vec3(0, 0, 1));
	}

	glm::vec3 Mathf::getUp(glm::quat rotation)
	{
		return glm::normalize(rotation * glm::vec3(0, 1, 0));
	}

	glm::vec3 Mathf::getRight(glm::quat rotation)
	{
		return glm::normalize(rotation * glm::vec3(1, 0, 0));
	}

	glm::vec3 Mathf::barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c)
	{
		glm::vec2 v0 = b - a, v1 = c - a, v2 = p - a;
		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);
		float denom = d00 * d11 - d01 * d01;
		float v = (d11 * d20 - d01 * d21) / denom;
		float w = (d00 * d21 - d01 * d20) / denom;
		float u = 1.0f - v - w;

		if (glm::isinf(v)) v = 0;
		if (glm::isinf(w)) w = 0;
		if (glm::isinf(u)) u = 0;

		return glm::vec3(u, v, w);
	}
}