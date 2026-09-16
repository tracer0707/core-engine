#pragma once

#include <cstdint>

namespace Core
{
    class Color
    {
      public:
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 1.0f;

        static Color RED;
        static Color GREEN;
        static Color BLUE;
        static Color WHITE;

        Color() = default;
        Color(float rr, float gg, float bb);
        Color(float rr, float gg, float bb, float aa);

        static uint32_t packRGB(Color value);
        static Color unpackRGB(uint32_t value);
        static uint32_t packRGBA(Color value);
        static Color unpackRGBA(uint32_t value);
    };
} // namespace Core