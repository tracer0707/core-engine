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
			const FrameBuffer* frameBuffer = nullptr;

			unsigned int width = 0;
			unsigned int height = 0;

		public:
			virtual int getContentType();

			void bind();

			unsigned int getNativeFrameBufferId();
			unsigned int getNativeColorTextureId();
			unsigned int getNativeDepthTextureId();

			unsigned int getWidth() { return width; }
			unsigned int getHeight() { return height; }

			void setSize(unsigned int width, unsigned int height);
	};
} // namespace Core