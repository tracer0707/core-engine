#include "AxisAlignedBox.h"
#include "Mathf.h"

namespace Core
{
    const AxisAlignedBox AxisAlignedBox::BOX_NULL;
    const AxisAlignedBox AxisAlignedBox::BOX_INFINITE(AxisAlignedBox::Extent::EXTENT_INFINITE);

    void AxisAlignedBox::merge(const AxisAlignedBox& rhs)
    {
        // Do nothing if rhs null, or this is infinite
        if ((rhs.mExtent == Extent::EXTENT_NULL) || (mExtent == Extent::EXTENT_INFINITE))
        {
            return;
        }
        // Otherwise if rhs is infinite, make this infinite, too
        else if (rhs.mExtent == Extent::EXTENT_INFINITE)
        {
            mExtent = Extent::EXTENT_INFINITE;
        }
        // Otherwise if current null, just take rhs
        else if (mExtent == Extent::EXTENT_NULL)
        {
            setExtents(rhs.mMinimum, rhs.mMaximum);
        }
        // Otherwise merge
        else
        {
            glm::vec3 min = mMinimum;
            glm::vec3 max = mMaximum;
            Mathf::makeCeil(max, rhs.mMaximum);
            Mathf::makeFloor(min, rhs.mMinimum);

            setExtents(min, max);
        }
    }

    void AxisAlignedBox::merge(const glm::vec3& point)
    {
        switch (mExtent)
        {
        case Extent::EXTENT_NULL: // if null, use this point
            setExtents(point, point);
            return;

        case Extent::EXTENT_FINITE:
            Mathf::makeCeil(mMaximum, point);
            Mathf::makeFloor(mMinimum, point);
            return;

        case Extent::EXTENT_INFINITE: // if infinite, makes no difference
            return;
        }

        assert(false && "Never reached");
    }

    AxisAlignedBox AxisAlignedBox::intersection(AxisAlignedBox& b2)
    {
        if (this->isNull() || b2.isNull())
        {
            return AxisAlignedBox();
        }
        else if (this->isInfinite())
        {
            return b2;
        }
        else if (b2.isInfinite())
        {
            return *this;
        }

        glm::vec3 intMin = mMinimum;
        glm::vec3 intMax = mMaximum;

        Mathf::makeCeil(intMin, b2.getMinimum());
        Mathf::makeFloor(intMax, b2.getMaximum());

        // Check intersection isn't null
        if (intMin.x < intMax.x &&
            intMin.y < intMax.y &&
            intMin.z < intMax.z)
        {
            return AxisAlignedBox(intMin, intMax);
        }

        return AxisAlignedBox();
    }

    float AxisAlignedBox::volume(void) const
    {
        switch (mExtent)
        {
        case Extent::EXTENT_NULL:
            return 0.0f;

        case Extent::EXTENT_FINITE:
        {
            glm::vec3 diff = mMaximum - mMinimum;
            return diff.x * diff.y * diff.z;
        }

        case Extent::EXTENT_INFINITE:
            return Mathf::POS_INFINITY;

        default: // shut up compiler
            assert(false && "Never reached");
            return 0.0f;
        }
    }

    glm::vec3 AxisAlignedBox::getSize(void) const
    {
        switch (mExtent)
        {
        case Extent::EXTENT_NULL:
            return glm::vec3(0, 0, 0);

        case Extent::EXTENT_FINITE:
            return mMaximum - mMinimum;

        case Extent::EXTENT_INFINITE:
            return glm::vec3(
                Mathf::POS_INFINITY,
                Mathf::POS_INFINITY,
                Mathf::POS_INFINITY);

        default: // shut up compiler
            assert(false && "Never reached");
            return glm::vec3(0, 0, 0);
        }
    }

    glm::vec3 AxisAlignedBox::getHalfSize(void) const
    {
        switch (mExtent)
        {
        case Extent::EXTENT_NULL:
            return glm::vec3(0, 0, 0);

        case Extent::EXTENT_FINITE:
            return (mMaximum - mMinimum) * 0.5f;

        case Extent::EXTENT_INFINITE:
            return glm::vec3(
                Mathf::POS_INFINITY,
                Mathf::POS_INFINITY,
                Mathf::POS_INFINITY);

        default: // shut up compiler
            assert(false && "Never reached");
            return glm::vec3(0, 0, 0);
        }
    }

    float AxisAlignedBox::squaredDistance(const glm::vec3& v) const
    {
        if (this->contains(v))
            return 0;
        else
        {
            glm::vec3 maxDist(0, 0, 0);

            if (v.x < mMinimum.x)
                maxDist.x = mMinimum.x - v.x;
            else if (v.x > mMaximum.x)
                maxDist.x = v.x - mMaximum.x;

            if (v.y < mMinimum.y)
                maxDist.y = mMinimum.y - v.y;
            else if (v.y > mMaximum.y)
                maxDist.y = v.y - mMaximum.y;

            if (v.z < mMinimum.z)
                maxDist.z = mMinimum.z - v.z;
            else if (v.z > mMaximum.z)
                maxDist.z = v.z - mMaximum.z;

            return Mathf::squaredLength(maxDist);
        }
    }
}