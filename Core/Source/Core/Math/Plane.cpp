#include "Plane.h"

#include <glm/glm.hpp>

namespace Core
{
    Plane::Plane()
    {
        normal = glm::vec3(0.0f);
        d = 0.0;
    }

    Plane::Plane(const Plane& rhs)
    {
        normal = rhs.normal;
        d = rhs.d;
    }

    Plane::Plane(const glm::vec3& rkNormal, float fConstant)
    {
        normal = rkNormal;
        d = -fConstant;
    }
    
    Plane::Plane(float a, float b, float c, float _d)
        : normal(a, b, c), d(_d)
    {
    }
    
    Plane::Plane(const glm::vec3& rkNormal, const glm::vec3& rkPoint)
    {
        redefine(rkNormal, rkPoint);
    }
    
    Plane::Plane(const glm::vec3& rkPoint0, const glm::vec3& rkPoint1,
        const glm::vec3& rkPoint2)
    {
        redefine(rkPoint0, rkPoint1, rkPoint2);
    }
    
    float Plane::getDistance(const glm::vec3& rkPoint) const
    {
        return glm::dot(normal, rkPoint) + d;
    }
    
    Plane::Side Plane::getSide(const glm::vec3& rkPoint) const
    {
        float fDistance = getDistance(rkPoint);

        if (fDistance < 0.0)
            return Plane::NEGATIVE_SIDE;

        if (fDistance > 0.0)
            return Plane::POSITIVE_SIDE;

        return Plane::NO_SIDE;
    }

    Plane::Side Plane::getSide(const AxisAlignedBox& box) const
    {
        if (box.isNull())
            return NO_SIDE;
        if (box.isInfinite())
            return BOTH_SIDE;

        return getSide(box.getCenter(), box.getHalfSize());
    }

    Plane::Side Plane::getSide(const glm::vec3& centre, const glm::vec3& halfSize) const
    {
        // Calculate the distance between box centre and the plane
        float dist = getDistance(centre);

        // Calculate the maximise allows absolute distance for
        // the distance between box centre and plane
        float maxAbsDist = glm::abs(glm::dot(normal, halfSize));

        if (dist < -maxAbsDist)
            return Plane::NEGATIVE_SIDE;

        if (dist > +maxAbsDist)
            return Plane::POSITIVE_SIDE;

        return Plane::BOTH_SIDE;
    }

    void Plane::redefine(const glm::vec3& rkPoint0, const glm::vec3& rkPoint1,
        const glm::vec3& rkPoint2)
    {
        glm::vec3 kEdge1 = rkPoint1 - rkPoint0;
        glm::vec3 kEdge2 = rkPoint2 - rkPoint0;
        normal = glm::cross(kEdge1, kEdge2);
        glm::normalize(normal);
        d = -glm::dot(normal, rkPoint0);
    }
    
    void Plane::redefine(const glm::vec3& rkNormal, const glm::vec3& rkPoint)
    {
        normal = rkNormal;
        d = -glm::dot(rkNormal, rkPoint);
    }
    
    glm::vec3 Plane::projectVector(const glm::vec3& p) const
    {
        // We know plane normal is unit length, so use simple method
        glm::mat3x3 xform;
        xform[0][0] = 1.0f - normal.x * normal.x;
        xform[0][1] = -normal.x * normal.y;
        xform[0][2] = -normal.x * normal.z;
        xform[1][0] = -normal.y * normal.x;
        xform[1][1] = 1.0f - normal.y * normal.y;
        xform[1][2] = -normal.y * normal.z;
        xform[2][0] = -normal.z * normal.x;
        xform[2][1] = -normal.z * normal.y;
        xform[2][2] = 1.0f - normal.z * normal.z;
        return xform * p;

    }
    //-----------------------------------------------------------------------
    float Plane::normalise(void)
    {
        float fLength = normal.length();

        // Will also work for zero-sized vectors, but will change nothing
        // We're not using epsilons because we don't need to.
        // Read http://www.ogre3d.org/forums/viewtopic.php?f=4&t=61259
        if (fLength > float(0.0f))
        {
            float fInvLength = 1.0f / fLength;
            normal *= fInvLength;
            d *= fInvLength;
        }

        return fLength;
    }
}