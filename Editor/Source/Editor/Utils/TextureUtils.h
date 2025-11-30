#pragma once

#include <vector>

struct FIBITMAP;
struct color_quad_u8;

namespace Editor
{
	class TextureUtils
	{
	public:
        static FIBITMAP* makeSquare(FIBITMAP* src);
        static FIBITMAP* rescale(FIBITMAP* src, int newW, int newH);
		static void copyPixels(std::vector<color_quad_u8>& dst, FIBITMAP* src, int width, int height);
	};
}