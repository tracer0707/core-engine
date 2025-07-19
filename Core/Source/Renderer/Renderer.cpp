#include "Renderer.h"
#include "RendererGL4.h"

#include "../System/DeviceContext.h"
#include "../Components/Camera.h"
#include "../Assets/Material.h"

namespace Core
{
	Renderer* Renderer::_current = nullptr;

	Renderer* Renderer::init(DeviceContext* ctx)
	{
		Renderer* renderer = nullptr;

#if CURRENT_RENDERER == GL4
		renderer = new RendererGL4(ctx);
#endif

		return renderer;
	}

	void Renderer::destroy(Renderer* renderer)
	{
		Material::destroyDefaultMaterial(renderer);

		delete renderer;
		renderer = nullptr;
	}

	Renderer::Renderer(DeviceContext* ctx)
	{
		_context = ctx;
	}

	Renderer::~Renderer()
	{
		_context = nullptr;
	}

	const void Renderer::makeCurrent()
	{
		_current = this;
	}

	Renderer* Renderer::current()
	{
		return _current;
	}
}