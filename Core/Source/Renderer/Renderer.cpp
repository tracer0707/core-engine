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

	Renderer* Renderer::singleton()
	{
		return _singleton;
	}
}