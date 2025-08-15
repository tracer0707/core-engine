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
		_defaultProgram = new Program();
	}

	Renderer::~Renderer()
	{
		delete _defaultProgram;

		_windowCtx = nullptr;
		_renderCtx = nullptr;
		_defaultProgram = nullptr;
	}
}