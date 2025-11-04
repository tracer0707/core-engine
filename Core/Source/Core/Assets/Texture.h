#pragma once

#include "Asset.h"

#include "../Renderer/Color.h"
#include "../Renderer/TextureFormat.h"
#include "../Shared/String.h"

struct FIBITMAP;

namespace Core
{
	class Renderer;

	class Texture : public Asset
	{
		friend class AssetManager;

	private:
		Texture(Renderer* renderer, FIBITMAP* bitmap, TextureFormat format);
		~Texture();

		Renderer* _renderer = nullptr;
		FIBITMAP* _bitmap = nullptr;
		
		int _width = 0;
		int _height = 0;

		unsigned int _nativeId = 0;

		TextureFormat _format = TextureFormat::RGBA8;

		void makeSquare();
		void updateSize();
		void rescale(int newW, int newH);

	public:
		unsigned int getNativeId() { return _nativeId; }

		int getWidth() { return _width; }
		int getHeight() { return _height; }

		void bind(const char* name, int slot);
	};
}