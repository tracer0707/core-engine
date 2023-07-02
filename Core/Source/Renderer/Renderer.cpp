#include "Renderer.h"
#include "RendererGL4.h"

#include "../Components/Camera.h"

namespace Core
{
	Renderer* _singleton = nullptr;

	void Renderer::init()
	{
#if CURRENT_RENDERER == GL4
		_singleton = new RendererGL4();
#endif
	}

	const Program& Program::operator=(const Program& left)
	{
		program = left.program;
		vertexShader = left.vertexShader;
		fragmentShader = left.fragmentShader;
		geometryShader = left.geometryShader;
		computeShader = left.computeShader;

		return *this;
	}

	const bool Program::operator==(const Program& left)
	{
		return program == left.program;
	}

	const Buffer& Buffer::operator=(const Buffer& left)
	{
		vbo = left.vbo;
		ibo = left.ibo;
		vertexArray = left.vertexArray;
		vertexArraySize = left.vertexArraySize;
		indexArray = left.indexArray;
		indexArraySize = left.indexArraySize;

		return *this;
	}

	const bool Buffer::operator==(const Buffer& left)
	{
		return vbo == left.vbo && ibo == left.ibo;
	}

	const void Renderer::frame(Camera* camera)
	{
		return void();
	}

	Renderer* Renderer::singleton()
	{
		return _singleton;
	}
}