#include "Texture.h"

#include "../Renderer/Renderer.h"
#include <GL/glew.h>

namespace Core
{
	Texture::Texture()
	{

	}

	Texture::~Texture()
	{
		if (isLoaded())
			unload();
	}

	const void Texture::unload()
	{
		if (_bitmap != nullptr)
			delete _bitmap;

		_bitmap = nullptr;

		Renderer::singleton()->deleteTexture(nativeId);
		nativeId = 0;

		Asset::unload();
	}

	Texture* Texture::loadFromFile(const char* fileName, TextureFormat fmt)
	{
		Texture* tex = new Texture();
		tex->format = fmt;

		FREE_IMAGE_FORMAT _fmt = FreeImage_GetFileType(fileName);
		FIBITMAP* texture = FreeImage_Load(_fmt, fileName);
		tex->_bitmap = FreeImage_ConvertTo32Bits(texture);
		tex->updateSize();
		FreeImage_Unload(texture);

		tex->makeSquare();

		if (fmt == TextureFormat::BC7)
		{
			color_quad_u8_vec pixels;
			uint32_t imageSize = (((tex->width + 3) & ~3) * ((tex->height + 3) & ~3) * 8) >> 3;
			unsigned char* newPixels = new unsigned char[imageSize];
			copyPixels(pixels, tex->_bitmap, tex->width, tex->height);
			int size = 0;
			bc7compress(pixels, tex->width, tex->height, newPixels, size, 1);
			pixels.clear();

			tex->nativeId = Renderer::singleton()->createTexture(newPixels, tex->getWidth(), tex->getHeight(), size, GL_COMPRESSED_RGBA_BPTC_UNORM);

			delete[] newPixels;
		}
		else
		{
			unsigned char* data = FreeImage_GetBits(tex->_bitmap);
			tex->nativeId = Renderer::singleton()->createTexture(data, tex->getWidth(), tex->getHeight(), 0, GL_RGBA8);
		}

		FreeImage_Unload(tex->_bitmap);

		tex->load();

		return tex;
	}

	Texture* Texture::loadFromBytes(unsigned char* data, int w, int h, int bpp, TextureFormat fmt)
	{
		Texture* tex = new Texture();
		tex->format = fmt;

		tex->_bitmap = FreeImage_Allocate(w, h, bpp * 8, 8, 8, 8);
		for (std::int32_t i = 0; i < h; ++i)
		{
			BYTE* bits = FreeImage_GetScanLine(tex->_bitmap, h - 1 - i);
			for (std::int32_t j = 0; j < w; ++j)
			{
				bits[0] = data[(i * w + j) * 4 + 2];
				bits[1] = data[(i * w + j) * 4 + 1];
				bits[2] = data[(i * w + j) * 4 + 0];
				bits[3] = data[(i * w + j) * 4 + 3];
				bits += 4;
			}
		}

		FIBITMAP* conv = FreeImage_ConvertTo32Bits(tex->_bitmap);
		FreeImage_Unload(tex->_bitmap);
		tex->_bitmap = conv;

		tex->updateSize();

		unsigned char* _data = FreeImage_GetBits(tex->_bitmap);
		tex->nativeId = Renderer::singleton()->createTexture(_data, tex->getWidth(), tex->getHeight(), 0, GL_RGBA8);

		FreeImage_Unload(tex->_bitmap);

		tex->load();

		return tex;
	}

	void Texture::makeSquare()
	{
		int w = FreeImage_GetWidth(_bitmap);
		int h = FreeImage_GetHeight(_bitmap);

		size_t u2 = 1; while (u2 < w) u2 *= 2;
		size_t v2 = 1; while (v2 < h) v2 *= 2;

		if (u2 > 1)
		{
			int pw = u2 / 2;
			if (w - pw < u2 - w)
				u2 = pw;
		}

		if (v2 > 1)
		{
			int ph = v2 / 2;
			if (h - ph < v2 - h)
				v2 = ph;
		}

		int w2 = std::min((int)u2, 4096);
		int h2 = std::min((int)v2, 4096);

		if (w != w2 || h != h2 || w != h)
		{
			int s = std::max(w2, h2);

			rescale(s, s);
		}
	}

	void Texture::copyPixels(color_quad_u8_vec& dst, FIBITMAP* src, int width, int height)
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

	void Texture::rescale(int newW, int newH)
	{
		FIBITMAP* scaled = FreeImage_Rescale(_bitmap, newW, newH, FREE_IMAGE_FILTER::FILTER_BOX);
		FreeImage_Unload(_bitmap);
		_bitmap = scaled;

		updateSize();
	}

	void Texture::updateSize()
	{
		width = FreeImage_GetWidth(_bitmap);
		height = FreeImage_GetHeight(_bitmap);
	}

	void Texture::bind(const char* name, int slot)
	{
		Renderer::singleton()->bindTexture(nativeId, name, slot);
	}
}