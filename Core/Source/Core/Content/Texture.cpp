#include "Texture.h"

#include "../Renderer/Renderer.h"

namespace Core
{
    Texture::Texture(Renderer* renderer, int width, int height, unsigned char* data, int size, TextureFormat format) : Content()
	{
		_renderer = renderer;
		_format = format;
        _size = size;
		_width = width;
        _height = height;
        _data = new unsigned char[_size];
        memcpy(_data, data, _size);

		if (format == TextureFormat::RGBA8)
		{
			_nativeId = _renderer->createTexture(_data, _width, _height, 0, format);
		}
		else if (format == TextureFormat::BC7)
		{
			_nativeId = _renderer->createTexture(_data, _width, _height, _size, format);
		}
		else
		{
			throw "Texture format is unsupported";
		}
	}

	Texture::~Texture()
	{
		if (_data != nullptr)
			delete[] _data;

		_renderer->deleteTexture(_nativeId);
		_data = nullptr;
		_renderer = nullptr;
        _width = 0;
        _height = 0;
		_nativeId = 0;
	}

	void Texture::bind(const char* name, int slot)
	{
		_renderer->bindTexture(_nativeId, name, slot);
	}
}