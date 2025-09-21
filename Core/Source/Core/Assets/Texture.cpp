#include "Texture.h"

#include <FreeImage.h>

#include "../Classes/bc7compressor.h"
#include "../Classes/TextureUtils.h"
#include "../Renderer/Renderer.h"

namespace Core
{
	Texture::Texture(Renderer* renderer, FIBITMAP* bitmap, TextureFormat format)
	{
		_renderer = renderer;
		_bitmap = bitmap;
		_format = format;

		makeSquare();
		updateSize();

		if (format == TextureFormat::RGBA8)
		{
			unsigned char* data = FreeImage_GetBits(_bitmap);
			_nativeId = _renderer->createTexture(data, _width, _height, 0, format);
		}
		else if (format == TextureFormat::BC7)
		{
			color_quad_u8_vec pixels;
			uint32_t imageSize = (((_width + 3) & ~3) * ((_height + 3) & ~3) * 8) >> 3;
			unsigned char* newPixels = new unsigned char[imageSize];
			TextureUtils::copyPixels(pixels, _bitmap, _width, _height);
			int size = 0;
			bc7compress(pixels, _width, _height, newPixels, size, 1);
			pixels.clear();

			_nativeId = _renderer->createTexture(newPixels, _width, _height, size, format);

			delete[] newPixels;
		}
		else
		{
			throw "Texture format is unsupported";
		}
	}

	Texture::~Texture()
	{
		if (_bitmap != nullptr)
			delete _bitmap;

		_renderer->deleteTexture(_nativeId);
		_bitmap = nullptr;
		_renderer = nullptr;
		_nativeId = 0;
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

	void Texture::rescale(int newW, int newH)
	{
		FIBITMAP* scaled = FreeImage_Rescale(_bitmap, newW, newH, FREE_IMAGE_FILTER::FILTER_BOX);
		FreeImage_Unload(_bitmap);
		_bitmap = scaled;

		updateSize();
	}

	void Texture::updateSize()
	{
		_width = FreeImage_GetWidth(_bitmap);
		_height = FreeImage_GetHeight(_bitmap);
	}

	void Texture::bind(const char* name, int slot)
	{
		_renderer->bindTexture(_nativeId, name, slot);
	}
}