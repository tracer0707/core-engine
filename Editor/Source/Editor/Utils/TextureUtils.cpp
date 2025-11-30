#include "TextureUtils.h"

#include "bc7compressor.h"

#include <FreeImage.h>

namespace Editor
{
    FIBITMAP* TextureUtils::makeSquare(FIBITMAP* src)
    {
        int w = FreeImage_GetWidth(src);
        int h = FreeImage_GetHeight(src);

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