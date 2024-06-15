#include "Renderer.h"
#include "RendererGL4.h"

#include "../System/DeviceContext.h"
#include "../Components/Camera.h"

namespace Core
{
	Renderer* _singleton = nullptr;

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

	void Renderer::init(DeviceContext* ctx)
	{
#if CURRENT_RENDERER == GL4
		_singleton = new RendererGL4(ctx);
#endif
	}

	Renderer::Renderer(DeviceContext* ctx)
	{
		context = ctx;
	}

	Renderer* Renderer::singleton()
	{
		return _singleton;
	}
}