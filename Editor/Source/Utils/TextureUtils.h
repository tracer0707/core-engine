#pragma once

#include <vector>

#include <Core/Shared/String.h>

struct FIBITMAP;
struct color_quad_u8;

namespace Core
{
	class Texture;
	class ContentManager;
} // namespace Core

namespace Editor
{
	class TextureUtils
	{
		public:
			static FIBITMAP* makeSquare(FIBITMAP* src);
			static FIBITMAP* rescale(FIBITMAP* src, int newW, int newH);
			static void copyPixels(std::vector<color_quad_u8>& dst, FIBITMAP* src, int width, int height);
			static Core::Texture* loadCompressed(Core::String fileName, Core::ContentManager* mgr);
	};
} // namespace Editor