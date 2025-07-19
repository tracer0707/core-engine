#include "RenderTexture.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/FrameBuffer.h"

namespace Core
{
	RenderTexture::RenderTexture(UInt32 width, UInt32 height)
	{
		this->width = width;
		this->height = height;

		frameBuffer = Renderer::current()->createFrameBuffer(width, height);
	}

	Core::RenderTexture::~RenderTexture()
	{
		if (frameBuffer != nullptr)
		{
			Renderer::current()->deleteFrameBuffer(frameBuffer);
			frameBuffer = nullptr;
		}
	}

	void RenderTexture::bind()
	{
		Renderer::current()->bindFrameBuffer(frameBuffer);
	}

	UInt32 RenderTexture::getNativeFrameBufferId()
	{
		return frameBuffer->frameBuffer;
	}

	UInt32 RenderTexture::getNativeColorTextureId()
	{
		return frameBuffer->colorTexture;
	}

	UInt32 RenderTexture::getNativeDepthTextureId()
	{
		return frameBuffer->depthTexture;
	}

	void RenderTexture::setSize(UInt32 width, UInt32 height)
	{
		this->width = width;
		this->height = height;

		Renderer::current()->deleteFrameBuffer(frameBuffer);
		frameBuffer = Renderer::current()->createFrameBuffer(width, height);
	}
}