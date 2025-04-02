#pragma once

#include <array>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Core
{
    class AxisAlignedBox
    {
    public:
        enum class Extent
        {
            EXTENT_NULL,
            EXTENT_FINITE,
            EXTENT_INFINITE
        };

    protected:
        glm::vec3 mMinimum;
        glm::vec3 mMaximum;
        Extent mExtent;

    public:

        enum class CornerEnum
        {
            FAR_LEFT_BOTTOM = 0,
            FAR_LEFT_TOP = 1,
            FAR_RIGHT_TOP = 2,
            FAR_RIGHT_BOTTOM = 3,
            NEAR_RIGHT_BOTTOM = 7,
            NEAR_LEFT_BOTTOM = 6,
            NEAR_LEFT_TOP = 5,
            NEAR_RIGHT_TOP = 4
        };
        typedef std::array<glm::vec3, 8> Corners;

        inline AxisAlignedBox() : mMinimum(glm::vec3(0, 0, 0)), mMaximum(glm::vec3(1, 1,1))
        {
            setMinimum(-0.5, -0.5, -0.5);
            setMaximum(0.5, 0.5, 0.5);
            mExtent = Extent::EXTENT_NULL;
        }
        inline AxisAlignedBox(Extent e) : mMinimum(glm::vec3(0, 0, 0)), mMaximum(glm::vec3(1, 1, 1))
        {
            setMinimum(-0.5, -0.5, -0.5);
            setMaximum(0.5, 0.5, 0.5);
            mExtent = e;
        }

        inline AxisAlignedBox(const AxisAlignedBox & rkBox) : mMinimum(glm::vec3(0, 0, 0)), mMaximum(glm::vec3(1, 1, 1))
        {
            if (rkBox.isNull())
                setNull();
            else if (rkBox.isInfinite())
                setInfinite();
            else
                setExtents( rkBox.mMinimum, rkBox.mMaximum );
        }

        inline AxisAlignedBox( const glm::vec3& min, const glm::vec3& max ) : mMinimum(glm::vec3(0, 0, 0)), mMaximum(glm::vec3(1, 1, 1))
        {
            setExtents( min, max );
        }

        inline AxisAlignedBox(
            float mx, float my, float mz,
            float Mx, float My, float Mz ) : mMinimum(glm::vec3(0, 0, 0)), mMaximum(glm::vec3(1, 1, 1))
        {
            setExtents( mx, my, mz, Mx, My, Mz );
        }

        AxisAlignedBox& operator=(const AxisAlignedBox& rhs)
        {
            if (rhs.isNull())
                setNull();
            else if (rhs.isInfinite())
                setInfinite();
            else
                setExtents(rhs.mMinimum, rhs.mMaximum);

            return *this;
        }

        inline glm::vec3& getMinimum(void)
        { 
            return mMinimum; 
        }

        inline glm::vec3& getMaximum(void)
        { 
            return mMaximum;
        }

        inline void setMinimum( const glm::vec3& vec )
        {
            mExtent = Extent::EXTENT_FINITE;
            mMinimum = vec;
        }

        inline void setMinimum( float x, float y, float z )
        {
            mExtent = Extent::EXTENT_FINITE;
            mMinimum.x = x;
            mMinimum.y = y;
            mMinimum.z = z;
        }

        inline void setMinimumX(float x)
        {
            mMinimum.x = x;
        }

        inline void setMinimumY(float y)
        {
            mMinimum.y = y;
        }

        inline void setMinimumZ(float z)
        {
            mMinimum.z = z;
        }

        inline void setMaximum( const glm::vec3& vec )
        {
            mExtent = Extent::EXTENT_FINITE;
            mMaximum = vec;
        }

        inline void setMaximum( float x, float y, float z )
        {
            mExtent = Extent::EXTENT_FINITE;
            mMaximum.x = x;
            mMaximum.y = y;
            mMaximum.z = z;
        }

        inline void setMaximumX( float x )
        {
            mMaximum.x = x;
        }

        inline void setMaximumY( float y )
        {
            mMaximum.y = y;
        }

        inline void setMaximumZ( float z )
        {
            mMaximum.z = z;
        }

        inline void setExtents( const glm::vec3& min, const glm::vec3& max )
        {
            assert( (min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
                "The minimum corner of the box must be less than or equal to maximum corner" );

            mExtent = Extent::EXTENT_FINITE;
            mMinimum = min;
            mMaximum = max;
        }

        inline void setExtents(
            float mx, float my, float mz,
            float Mx, float My, float Mz )
        {
            assert( (mx <= Mx && my <= My && mz <= Mz) &&
                "The minimum corner of the box must be less than or equal to maximum corner" );

            mExtent = Extent::EXTENT_FINITE;

            mMinimum.x = mx;
            mMinimum.y = my;
            mMinimum.z = mz;

            mMaximum.x = Mx;
            mMaximum.y = My;
            mMaximum.z = Mz;
        }

        inline Corners getAllCorners(void) const
        {
            assert( (mExtent == Extent::EXTENT_FINITE) && "Can't get corners of a null or infinite AAB" );

            Corners corners;

            corners[0] = getCorner(CornerEnum::FAR_LEFT_BOTTOM);
            corners[1] = getCorner(CornerEnum::FAR_LEFT_TOP);
            corners[2] = getCorner(CornerEnum::FAR_RIGHT_TOP);
            corners[3] = getCorner(CornerEnum::FAR_RIGHT_BOTTOM);

            corners[4] = getCorner(CornerEnum::NEAR_RIGHT_TOP);
            corners[5] = getCorner(CornerEnum::NEAR_LEFT_TOP);
            corners[6] = getCorner(CornerEnum::NEAR_LEFT_BOTTOM);
            corners[7] = getCorner(CornerEnum::NEAR_RIGHT_BOTTOM);

            return corners;
        }

        glm::vec3 getCorner(CornerEnum cornerToGet) const
        {
            switch(cornerToGet)
            {
            case CornerEnum::FAR_LEFT_BOTTOM:
                return mMinimum;
            case CornerEnum::FAR_LEFT_TOP:
                return glm::vec3(mMinimum.x, mMaximum.y, mMinimum.z);
            case CornerEnum::FAR_RIGHT_TOP:
                return glm::vec3(mMaximum.x, mMaximum.y, mMinimum.z);
            case CornerEnum::FAR_RIGHT_BOTTOM:
                return glm::vec3(mMaximum.x, mMinimum.y, mMinimum.z);
            case CornerEnum::NEAR_RIGHT_BOTTOM:
                return glm::vec3(mMaximum.x, mMinimum.y, mMaximum.z);
            case CornerEnum::NEAR_LEFT_BOTTOM:
                return glm::vec3(mMinimum.x, mMinimum.y, mMaximum.z);
            case CornerEnum::NEAR_LEFT_TOP:
                return glm::vec3(mMinimum.x, mMaximum.y, mMaximum.z);
            case CornerEnum::NEAR_RIGHT_TOP:
                return mMaximum;
            default:
                return glm::vec3();
            }
        }

        void merge(const AxisAlignedBox& rhs);

        void merge(const glm::vec3& point);

        inline void transform(const glm::mat4x4& matrix)
        {
            if (mExtent != Extent::EXTENT_FINITE)
                return;

            glm::vec3 centre = getCenter();
            glm::vec3 halfSize = getHalfSize();

            glm::vec3 newCentre = matrix * glm::vec4(centre, 1.0f);
            glm::vec3 newHalfSize(
                abs(matrix[0].x) * halfSize.x + abs(matrix[1].x) * halfSize.y + abs(matrix[2].x) * halfSize.z,
                abs(matrix[0].y) * halfSize.x + abs(matrix[1].y) * halfSize.y + abs(matrix[2].y) * halfSize.z,
                abs(matrix[0].z) * halfSize.x + abs(matrix[1].z) * halfSize.y + abs(matrix[2].z) * halfSize.z);

            setExtents(newCentre - newHalfSize, newCentre + newHalfSize);
        }

        inline void setNull()
        {
            mExtent = Extent::EXTENT_NULL;
        }

        inline bool isNull(void) const
        {
            return (mExtent == Extent::EXTENT_NULL);
        }

        bool isFinite(void) const
        {
            return (mExtent == Extent::EXTENT_FINITE);
        }

        inline void setInfinite()
        {
            mExtent = Extent::EXTENT_INFINITE;
        }

        bool isInfinite(void) const
        {
            return (mExtent == Extent::EXTENT_INFINITE);
        }

        inline bool intersects(const AxisAlignedBox& b2) const
        {
            if (this->isNull() || b2.isNull())
                return false;

            if (this->isInfinite() || b2.isInfinite())
                return true;

            if (mMaximum.x < b2.mMinimum.x)
                return false;
            if (mMaximum.y < b2.mMinimum.y)
                return false;
            if (mMaximum.z < b2.mMinimum.z)
                return false;

            if (mMinimum.x > b2.mMaximum.x)
                return false;
            if (mMinimum.y > b2.mMaximum.y)
                return false;
            if (mMinimum.z > b2.mMaximum.z)
                return false;

            return true;
        }

        AxisAlignedBox intersection(AxisAlignedBox& b2);

        float volume(void) const;

        inline void scale(const glm::vec3& s)
        {
            if (mExtent != Extent::EXTENT_FINITE)
                return;

            glm::vec3 min = mMinimum * s;
            glm::vec3 max = mMaximum * s;
            setExtents(min, max);
        }

        bool intersects(const glm::vec3& v) const
        {
            switch (mExtent)
            {
            case Extent::EXTENT_NULL:
                return false;

            case Extent::EXTENT_FINITE:
                return(v.x >= mMinimum.x  &&  v.x <= mMaximum.x  && 
                    v.y >= mMinimum.y  &&  v.y <= mMaximum.y  && 
                    v.z >= mMinimum.z  &&  v.z <= mMaximum.z);

            case Extent::EXTENT_INFINITE:
                return true;

            default: // shut up compiler
                assert( false && "Never reached" );
                return false;
            }
        }

        std::pair<bool, float> intersects(glm::vec3 start, glm::vec3 dir)
        {
            if (isNull()) return std::pair<bool, float>(false, (float)0);
            if (isInfinite()) return std::pair<bool, float>(true, (float)0);

            float lowt = 0.0f;
            float t;
            bool hit = false;
            glm::vec3 hitpoint;
            glm::vec3 min = getMinimum();
            glm::vec3 max = getMaximum();
            glm::vec3 rayorig = start;
            glm::vec3 raydir = dir;

            if (intersects(rayorig))
            {
                return std::pair<bool, float>(true, (float)0);
            }

            if (rayorig.x <= min.x && raydir.x > 0)
            {
                t = (min.x - rayorig.x) / raydir.x;

                hitpoint = rayorig + raydir * t;
                if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
            
            if (rayorig.x >= max.x && raydir.x < 0)
            {
                t = (max.x - rayorig.x) / raydir.x;

                hitpoint = rayorig + raydir * t;
                if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
            
            if (rayorig.y <= min.y && raydir.y > 0)
            {
                t = (min.y - rayorig.y) / raydir.y;

                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
            
            if (rayorig.y >= max.y && raydir.y < 0)
            {
                t = (max.y - rayorig.y) / raydir.y;

                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
            
            if (rayorig.z <= min.z && raydir.z > 0)
            {
                t = (min.z - rayorig.z) / raydir.z;

                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
            
            if (rayorig.z >= max.z && raydir.z < 0)
            {
                t = (max.z - rayorig.z) / raydir.z;

                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }

            return std::pair<bool, float>(hit, (float)lowt);
        }

        glm::vec3 getCenter(void) const
        {
            assert( (mExtent == Extent::EXTENT_FINITE) && "Can't get center of a null or infinite AAB" );

            return glm::vec3(
                (mMaximum.x + mMinimum.x) * 0.5f,
                (mMaximum.y + mMinimum.y) * 0.5f,
                (mMaximum.z + mMinimum.z) * 0.5f);
        }

        glm::vec3 getSize(void) const;

        glm::vec3 getHalfSize(void) const;

        float getRadius()
        {
            glm::vec3 hs = getHalfSize();
            return std::max(std::max(hs.x, hs.y), hs.z);
        }

        float getMinRadius()
        {
            glm::vec3 hs = getHalfSize();
            return std::min(std::min(hs.x, hs.y), hs.z);
        }

        bool contains(const glm::vec3& v) const
        {
            if (isNull())
                return false;
            if (isInfinite())
                return true;

            return mMinimum.x <= v.x && v.x <= mMaximum.x &&
                    mMinimum.y <= v.y && v.y <= mMaximum.y &&
                    mMinimum.z <= v.z && v.z <= mMaximum.z;
        }
        
        float squaredDistance(const glm::vec3& v) const;
            
        float distance (const glm::vec3& v) const
        {
            return sqrt(squaredDistance(v));
        }

        bool contains(const AxisAlignedBox& other) const
        {
            if (other.isNull() || this->isInfinite())
                return true;

            if (this->isNull() || other.isInfinite())
                return false;

            return this->mMinimum.x <= other.mMinimum.x &&
                    this->mMinimum.y <= other.mMinimum.y &&
                    this->mMinimum.z <= other.mMinimum.z &&
                    other.mMaximum.x <= this->mMaximum.x &&
                    other.mMaximum.y <= this->mMaximum.y &&
                    other.mMaximum.z <= this->mMaximum.z;
        }

        bool operator== (const AxisAlignedBox& rhs) const
        {
            if (this->mExtent != rhs.mExtent)
                return false;

            if (!this->isFinite())
                return true;

            return this->mMinimum == rhs.mMinimum &&
                    this->mMaximum == rhs.mMaximum;
        }

        bool operator!= (const AxisAlignedBox& rhs) const
        {
            return !(*this == rhs);
        }

        static const AxisAlignedBox BOX_NULL;
        static const AxisAlignedBox BOX_INFINITE;
    };
}