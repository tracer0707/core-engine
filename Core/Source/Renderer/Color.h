#pragma once

#include <cstdint>

namespace Core
{
	class Color
	{
	public:
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 1;

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
}