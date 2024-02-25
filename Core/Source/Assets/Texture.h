#pragma once

#include <FreeImage.h>

#include "../Renderer/Color.h"
#include "../Classes/bc7compressor.h"

namespace Core
{
	enum class TextureFormat
	{
		RGBA,
		BC7
	};

	class Texture
	{
	private:
		FIBITMAP* _bitmap = nullptr;
		
		int width = 0;
		int height = 0;

		int nativeId = 0;

		TextureFormat format = TextureFormat::RGBA;

		void makeSquare();
		static void copyPixels(color_quad_u8_vec& dst, FIBITMAP* src, int width, int height);

	public:
		Texture();
		~Texture();

		static Texture* loadFromFile(const char* fileName, TextureFormat fmt);
		static Texture* loadFromBytes(unsigned char* data, int w, int h, int bpp, TextureFormat fmt);

		void rescale(int newW, int newH);

		void updateSize();

		int getWidth() { return width; }
		int getHeight() { return height; }

		void bind(const char* name, int slot);
	};
}