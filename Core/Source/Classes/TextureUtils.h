#pragma once

#include <vector>

struct FIBITMAP;
struct color_quad_u8;

namespace Core
{
	class TextureUtils
	{
	public:
		static void copyPixels(std::vector<color_quad_u8>& dst, FIBITMAP* src, int width, int height);
	};
}