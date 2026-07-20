#pragma once

#include "Content.h"

namespace Core
{
	class FrameBuffer;
	class Renderer;

	class RenderTexture : public Content
	{
			friend class ContentManager;

		private:
			RenderTexture(Renderer* renderer, unsigned int width, unsigned int height);
			~RenderTexture();

			Renderer* _renderer = nullptr;
			const FrameBuffer* _frameBuffer = nullptr;

			unsigned int _width = 0;
			unsigned int _height = 0;

		public:
			virtual ContentType getContentType() const { return ContentType::RenderTexture; }

			void bind();

			unsigned int getNativeFrameBufferId();
			unsigned int getNativeColorTextureId();
			unsigned int getNativeDepthTextureId();

			unsigned int getWidth() const { return _width; }
			unsigned int getHeight() const { return _height; }

			void setSize(unsigned int width, unsigned int height);
	};
} // namespace Core