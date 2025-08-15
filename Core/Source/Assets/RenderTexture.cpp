#include "RenderTexture.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/FrameBuffer.h"

namespace Core
{
	RenderTexture::RenderTexture(Renderer* renderer, unsigned int width, unsigned int height)
	{
		_renderer = renderer;

		this->width = width;
		this->height = height;

		frameBuffer = _renderer->createFrameBuffer(width, height);
	}

	Core::RenderTexture::~RenderTexture()
	{
		if (frameBuffer != nullptr)
		{
			_renderer->deleteFrameBuffer(frameBuffer);
			frameBuffer = nullptr;
		}

		_renderer = nullptr;
	}

	void RenderTexture::bind()
	{
		_renderer->bindFrameBuffer(frameBuffer);
	}

	unsigned int RenderTexture::getNativeFrameBufferId()
	{
		return frameBuffer->frameBuffer;
	}

	unsigned int RenderTexture::getNativeColorTextureId()
	{
		return frameBuffer->colorTexture;
	}

	unsigned int RenderTexture::getNativeDepthTextureId()
	{
		return frameBuffer->depthTexture;
	}

	void RenderTexture::setSize(unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;

		_renderer->deleteFrameBuffer(frameBuffer);
		frameBuffer = _renderer->createFrameBuffer(width, height);
	}
}