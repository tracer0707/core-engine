#pragma once

#include <cstdint>
#include <unordered_map>

#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>

namespace Core
{
	class Renderer;
}

namespace Editor
{
	class Texture
	{
		private:
			Core::Renderer* _renderer = nullptr;

			uint32_t _nativeId = 0u;
			uint32_t _width = 0u;
			uint32_t _height = 0u;

		public:
			Texture(Core::Renderer* renderer, unsigned char* data, uint32_t width, uint32_t height);
			~Texture();

			const uint32_t& getWidth() const { return _width; }
			const uint32_t& getHeight() const { return _height; }
			const uint32_t& getNativeId() const { return _nativeId; }
	};
}