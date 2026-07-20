#include "RenderTexture.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/FrameBuffer.h"

namespace Core
{
	RenderTexture::RenderTexture(Renderer* renderer, unsigned int width, unsigned int height) : Content()
	{
		_renderer = renderer;

		_width = width;
		_height = height;

		_frameBuffer = _renderer->createFrameBuffer(width, height);
	}

	Core::RenderTexture::~RenderTexture()
	{
		if (_frameBuffer != nullptr)
		{
			_renderer->deleteFrameBuffer(_frameBuffer);
			_frameBuffer = nullptr;
		}

		_renderer = nullptr;
	}

	void RenderTexture::bind()
	{
		_renderer->bindFrameBuffer(_frameBuffer);
	}

	unsigned int RenderTexture::getNativeFrameBufferId()
	{
		return _frameBuffer->frameBuffer;
	}

	unsigned int RenderTexture::getNativeColorTextureId()
	{
		return _frameBuffer->colorBuffer;
	}

	unsigned int RenderTexture::getNativeDepthTextureId()
	{
		return _frameBuffer->depthBuffer;
	}

	void RenderTexture::setSize(unsigned int width, unsigned int height)
	{
		_width = std::max(width, 1u);
		_height = std::max(height, 1u);

		if (_frameBuffer != nullptr)
		{
			_renderer->deleteFrameBuffer(_frameBuffer);
		}

		_frameBuffer = _renderer->createFrameBuffer(_width, _height);
	}
} // namespace Core