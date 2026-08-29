#include "Texture2D.h"

#include <cstring>

#include "../Renderer/Renderer.h"

namespace Core
{
	Texture2D::Texture2D(Renderer* renderer, int width, int height, unsigned char* data, int size, TextureFormat format) : Content()
	{
		_renderer = renderer;
		_format = format;
		_size = size;
		_width = width;
		_height = height;

		if (format == TextureFormat::RGBA8)
		{
			_nativeId = _renderer->createTexture(data, _width, _height, 0, format);
		}
		else if (format == TextureFormat::BC7)
		{
			_nativeId = _renderer->createTexture(data, _width, _height, _size, format);
		}
		else
		{
			throw "Texture format is unsupported";
		}
	}

	Texture2D::~Texture2D()
	{
		_renderer->deleteTexture(_nativeId);
		_renderer = nullptr;
		_width = 0;
		_height = 0;
		_nativeId = 0;
	}

	void Texture2D::bind(int slot)
	{
		_renderer->bindTexture(_nativeId, slot);
	}
} // namespace Core