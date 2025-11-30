#pragma once

#include "Content.h"

#include "../Renderer/Color.h"
#include "../Renderer/TextureFormat.h"
#include "../Shared/String.h"

namespace Core
{
	class Renderer;

	class Texture : public Content
	{
    friend class ContentManager;

	private:
		Texture(Renderer* renderer, int width, int height, unsigned char* data, int size, TextureFormat format);
		~Texture();

		Renderer* _renderer = nullptr;
        unsigned char* _data = nullptr;
        int _size = 0;
		
		int _width = 0;
		int _height = 0;

		unsigned int _nativeId = 0;

		TextureFormat _format = TextureFormat::RGBA8;

	public:
		unsigned int getNativeId() { return _nativeId; }

		int getWidth() { return _width; }
		int getHeight() { return _height; }

		void bind(const char* name, int slot);
	};
}