#pragma once

#include <vector>
#include <filesystem>

#include <Core/Shared/String.h>

struct FIBITMAP;
struct color_quad_u8;

namespace fs = std::filesystem;

namespace Core
{
	class Renderer;
	class FrameBuffer;
} // namespace Core

namespace Editor
{
	class TextureUtils
	{
		public:
			static FIBITMAP* makeSquare(FIBITMAP* src);
			static FIBITMAP* rescale(FIBITMAP* src, int newW, int newH);
			static void copyPixels(std::vector<color_quad_u8>& dst, FIBITMAP* src, int width, int height);
			static bool saveFrameBuffer(Core::Renderer* renderer, const Core::FrameBuffer* frameBuffer, const fs::path& fileName);
	};
} // namespace Editor