#include "Texture.h"

#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/TextureFormat.h>

namespace Editor
{
	Texture::Texture(Core::Renderer* renderer, unsigned char* data, uint32_t width, uint32_t height)
	{
		_renderer = renderer;
		_width = width;
		_height = height;
		_nativeId = renderer->createTexture(data, width, height, 0, Core::TextureFormat::RGBA8);
	}

	Texture::~Texture()
	{
		if (_nativeId != 0u && _renderer != nullptr)
		{
			_renderer->deleteTexture(_nativeId);
		}
	}
} // namespace Editor