#include "TextureUtils.h"

#include "bc7compressor.h"

#include <FreeImage.h>

namespace Core
{
	void TextureUtils::copyPixels(std::vector<color_quad_u8>& dst, FIBITMAP* src, int width, int height)
	{
		BYTE* pixels = (BYTE*)FreeImage_GetBits(src);
		FIBITMAP* alphaChannel = FreeImage_GetChannel(src, FREE_IMAGE_COLOR_CHANNEL::FICC_ALPHA);
		BYTE* bits = FreeImage_GetBits(alphaChannel);

		int pixelsCount = width * height;
		dst.resize(pixelsCount);

		int pos = 0;
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				color_quad_u8 pixel;
				RGBQUAD rgb;
				FreeImage_GetPixelColor(src, j, i, &rgb);

				BYTE alpha = 255;
				if (alphaChannel != nullptr)
					alpha = pixels[pos * 4 + 3];

				pixel.set(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue, alpha);
				dst[pos] = pixel;
				++pos;
			}
		}

		if (alphaChannel != nullptr)
			FreeImage_Unload(alphaChannel);
	}
}