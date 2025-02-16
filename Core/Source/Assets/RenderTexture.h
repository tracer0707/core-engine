#pragma once

#include "../Config.h"
#include "Asset.h"

namespace Core
{
	class FrameBuffer;

	class RenderTexture : public Asset
	{
	private:
		const FrameBuffer* frameBuffer = nullptr;

		UInt32 width = 0;
		UInt32 height = 0;

	public:
		RenderTexture(UInt32 width, UInt32 height);
		~RenderTexture();

		void bind();

		UInt32 getNativeFrameBufferId();
		UInt32 getNativeColorTextureId();
		UInt32 getNativeDepthTextureId();

		UInt32 getWidth() { return width; }
		UInt32 getHeight() { return height; }

		void setSize(UInt32 width, UInt32 height);
	};
}