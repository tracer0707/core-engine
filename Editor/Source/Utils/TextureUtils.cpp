#include "TextureUtils.h"

#include <FreeImage.h>

#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/FrameBuffer.h>

namespace Editor
{
	FIBITMAP* TextureUtils::makeSquare(FIBITMAP* src)
	{
		unsigned int w = FreeImage_GetWidth(src);
		unsigned int h = FreeImage_GetHeight(src);

		size_t u2 = 1;
		while (u2 < w)
			u2 *= 2;
		size_t v2 = 1;
		while (v2 < h)
			v2 *= 2;

		if (u2 > 1)
		{
			int pw = u2 / 2;
			if (w - pw < u2 - w) u2 = pw;
		}

		if (v2 > 1)
		{
			int ph = v2 / 2;
			if (h - ph < v2 - h) v2 = ph;
		}

		int w2 = std::min((int)u2, 4096);
		int h2 = std::min((int)v2, 4096);
		int s = std::max(w2, h2);

		return rescale(src, s, s);
	}

	FIBITMAP* TextureUtils::rescale(FIBITMAP* src, int newW, int newH)
	{
		return FreeImage_Rescale(src, newW, newH, FREE_IMAGE_FILTER::FILTER_BOX);
	}

	void TextureUtils::swapRedBlueChannels(uint8_t* pixels, uint32_t width, uint32_t height, uint32_t pitch)
	{
		for (uint32_t y = 0; y < height; ++y)
		{
			uint8_t* row = pixels + y * pitch;

			for (uint32_t x = 0; x < width; ++x)
			{
				uint8_t* pixel = row + x * 4;
				std::swap(pixel[0], pixel[2]);
			}
		}
	}

	bool TextureUtils::saveFrameBuffer(Core::Renderer* renderer, const Core::FrameBuffer* frameBuffer, const fs::path& fileName)
	{
		std::vector<unsigned char> pixels = renderer->readFrameBufferPixels(frameBuffer);
		const size_t expectedSize = static_cast<size_t>(frameBuffer->width) * frameBuffer->height * 4u;
		if (pixels.size() != expectedSize)
		{
			return false;
		}

		FIBITMAP* image = FreeImage_Allocate(frameBuffer->width, frameBuffer->height, 32);
		if (image == nullptr)
		{
			return false;
		}

		for (unsigned int y = 0; y < frameBuffer->height; ++y)
		{
			BYTE* destination = FreeImage_GetScanLine(image, y);
			const unsigned char* source = pixels.data() + static_cast<size_t>(y) * frameBuffer->width * 4u;

			for (unsigned int x = 0; x < frameBuffer->width; ++x)
			{
				destination[x * 4u + 0u] = source[x * 4u + 2u];
				destination[x * 4u + 1u] = source[x * 4u + 1u];
				destination[x * 4u + 2u] = source[x * 4u + 0u];
				destination[x * 4u + 3u] = source[x * 4u + 3u];
			}
		}

		bool result = false;
#ifdef _WIN32
		result = FreeImage_SaveU(FIF_PNG, image, fileName.wstring().c_str()) != FALSE;
#else
		result = FreeImage_Save(FIF_PNG, image, fileName.c_str()) != FALSE;
#endif
		FreeImage_Unload(image);
		return result;
	}
} // namespace Editor