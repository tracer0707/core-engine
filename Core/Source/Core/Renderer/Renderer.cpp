#include "Renderer.h"

#include "RendererGL4.h"
#include "Program.h"

#include "../Config.h"
#include "../Components/Camera.h"

namespace Core
{
	Renderer* Renderer::init(void* windowCtx)
	{
		Renderer* renderer = nullptr;

#if CURRENT_RENDERER == GL4
		renderer = new RendererGL4(windowCtx);
#endif

		return renderer;
	}

	void Renderer::destroy(Renderer* renderer)
	{
		delete renderer;
	}

	Renderer::Renderer(void* windowCtx)
	{
		_windowCtx = windowCtx;
	}

	Renderer::~Renderer()
	{
		_windowCtx = nullptr;
		_renderCtx = nullptr;
	}

	Program* Renderer::getShaderProgram(String name) const
	{
		auto it = _shaderPrograms.find(name);
		if (it != _shaderPrograms.end())
		{
			return it->second;
		}
		return nullptr;
	}

	std::vector<unsigned char> Renderer::generateDefaultTextureData(unsigned int width, unsigned int height, unsigned int cellSize)
	{
		std::vector<unsigned char> data(width * height * 4);

		for (unsigned int y = 0; y < height; ++y)
		{
			for (unsigned int x = 0; x < width; ++x)
			{
				const bool checker = ((x / cellSize) + (y / cellSize)) % 2 == 0;

				const unsigned char value = checker ? 175 : 80;

				const size_t index = (y * width + x) * 4;

				// BGRA
				data[index + 0] = value; // B
				data[index + 1] = value; // G
				data[index + 2] = value; // R
				data[index + 3] = 255;	 // A
			}
		}

		return data;
	}
} // namespace Core