#pragma once

#include <glm/vec3.hpp>
#include "AxisAlignedBox.h"

namespace Core
{
    class Plane
    {
    public:
        glm::vec3 normal;
        float d;

    public:
        
        Plane();
        Plane(const Plane& rhs);
        Plane(const glm::vec3& rkNormal, float fConstant);
        Plane(float a, float b, float c, float d);
        Plane(const glm::vec3& rkNormal, const glm::vec3& rkPoint);
        Plane(const glm::vec3& rkPoint0, const glm::vec3& rkPoint1,
            const glm::vec3& rkPoint2);

        enum Side
        {
            NO_SIDE,
            POSITIVE_SIDE,
            NEGATIVE_SIDE,
            BOTH_SIDE
        };

        Side getSide(const glm::vec3& rkPoint) const;

        Side getSide(const AxisAlignedBox& rkBox) const;

        Side getSide(const glm::vec3& centre, const glm::vec3& halfSize) const;

        float getDistance(const glm::vec3& rkPoint) const;

        void redefine(const glm::vec3& rkPoint0, const glm::vec3& rkPoint1,
            const glm::vec3& rkPoint2);

        void redefine(const glm::vec3& rkNormal, const glm::vec3& rkPoint);

        glm::vec3 projectVector(const glm::vec3& v) const;

        float normalise(void);

        Plane operator - () const
        {
            return Plane(-(normal.x), -(normal.y), -(normal.z), -d);
        }

        bool operator==(const Plane& rhs) const
        {
            return (rhs.d == d && rhs.normal == normal);
        }

        bool operator!=(const Plane& rhs) const
        {
            return (rhs.d != d || rhs.normal != normal);
        }
    };
}