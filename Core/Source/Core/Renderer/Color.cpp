#include "Color.h"

namespace Core
{
    Color Color::RED = Color(1.0f, 0.0f, 0.0f, 1.0f);
    Color Color::GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);
    Color Color::BLUE = Color(0.0f, 0.0f, 1.0f, 1.0f);
    Color Color::WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);

    Color::Color(float rr, float gg, float bb)
    {
        r = rr;
        g = gg;
        b = bb;
    }

    Color::Color(float rr, float gg, float bb, float aa)
    {
        r = rr;
        g = gg;
        b = bb;
        a = aa;
    }

    uint32_t Color::packRGB(Color value)
    {
        int iR = value.r * 255.0f;
        int iG = value.g * 255.0f;
        int iB = value.b * 255.0f;

        return (iR << 16) | (iG << 8) | (iB);
    }

    Color Color::unpackRGB(uint32_t value)
    {
        int iR = (value >> 16) & 0xff;
        int iG = (value >> 8) & 0xff;
        int iB = (value) & 0xff;

        float r = 1.0f / 255.0f * iR;
        float g = 1.0f / 255.0f * iG;
        float b = 1.0f / 255.0f * iB;

        return Color(r, g, b, 1.0f);
    }

    uint32_t Color::packRGBA(Color value)
    {
        int iR = value.r * 255.0f;
        int iG = value.g * 255.0f;
        int iB = value.b * 255.0f;
        int iA = value.a * 255.0f;

        return (iR << 24) | (iG << 16) | (iB << 8) | iA;
    }

    Color Color::unpackRGBA(uint32_t value)
    {
        int iR = (value >> 24) & 0xff;
        int iG = (value >> 16) & 0xff;
        int iB = (value >> 8) & 0xff;
        int iA = (value) & 0xff;

        float r = 1.0f / 255.0f * iR;
        float g = 1.0f / 255.0f * iG;
        float b = 1.0f / 255.0f * iB;
        float a = 1.0f / 255.0f * iA;

        return Color(r, g, b, a);
    }
} // namespace Core