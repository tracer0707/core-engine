#include "Texture.h"

#include <cstring>

#include <FreeImage.h>

#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/TextureFormat.h>
#include <Core/Shared/String.h>

#ifdef _WIN32
#include <Core/Shared/Path.h>
#endif

#include "../Utils/TextureUtils.h"

namespace Editor
{
	Texture::Texture(Core::Renderer* renderer, uint32_t width, uint32_t height, uint32_t nativeId)
	{
		_renderer = renderer;
		_width = width;
		_height = height;
		_nativeId = nativeId;
	}

	Texture::~Texture()
	{
		if (_nativeId != 0u && _renderer != nullptr)
		{
			_renderer->deleteTexture(_nativeId);
		}
	}

	Texture* Texture::loadFromFile(Core::Renderer* renderer, Core::String fileName)
	{
		int w, h, size;

		FREE_IMAGE_FORMAT _fmt;
		FIBITMAP* texture;
#ifdef _WIN32
		const std::wstring wideFileName = fileName.wide_str();
		_fmt = FreeImage_GetFileTypeU(wideFileName.c_str());
		texture = FreeImage_LoadU(_fmt, wideFileName.c_str());
#else
		_fmt = FreeImage_GetFileType(fileName.c_str());
		texture = FreeImage_Load(_fmt, fileName.c_str());
#endif
		if (texture == nullptr)
			return nullptr;

		FIBITMAP* convert = TextureUtils::makeSquare(texture);
		FreeImage_Unload(texture);
		texture = convert;

		if (FreeImage_GetBPP(texture) != 32)
		{
			FIBITMAP* convert = FreeImage_ConvertTo32Bits(texture);
			FreeImage_Unload(texture);
			texture = convert;
		}

		w = FreeImage_GetWidth(texture);
		h = FreeImage_GetHeight(texture);

		unsigned char* src = FreeImage_GetBits(texture);
		unsigned int bpp = FreeImage_GetBPP(texture) / 8;
		unsigned int pitch = FreeImage_GetPitch(texture);
		size = w * h * bpp;
		unsigned char* dst = new unsigned char[size];

		for (unsigned y = 0; y < h; y++)
		{
			memcpy(dst + y * w * bpp, src + y * pitch, w * bpp);
		}

		uint32_t nativeId = renderer->createTexture(dst, w, h, 0, Core::TextureFormat::RGBA8);

		return new Texture(renderer, w, h, nativeId);
	}
} // namespace Editor