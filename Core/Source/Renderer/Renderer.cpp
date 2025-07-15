#include "Renderer.h"
#include "RendererGL4.h"

#include "../System/DeviceContext.h"
#include "../Components/Camera.h"
#include "../Assets/Material.h"

namespace Core
{
	Renderer* _singleton = nullptr;

	void Renderer::init(DeviceContext* ctx)
	{
#if CURRENT_RENDERER == GL4
		_singleton = new RendererGL4(ctx);
#endif

		Material::defaultMaterial = new Material();
	}

	void Renderer::destroy()
	{
		delete Material::defaultMaterial;
		Material::defaultMaterial = nullptr;

		if (_singleton != nullptr)
		{
			delete _singleton;
			_singleton = nullptr;
		}
	}

	Renderer::Renderer(DeviceContext* ctx)
	{
		context = ctx;
	}

	Renderer::~Renderer()
	{
		context = nullptr;
	}

	Renderer* Renderer::singleton()
	{
		return _singleton;
	}
}