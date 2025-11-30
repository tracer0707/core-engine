#include "Modifier.h"

namespace Editor
{
	Modifier::Modifier(uint32_t name)
	{
		_name = name;
	}

	Modifier::~Modifier()
	{
		_renderer = nullptr;
		_scene = nullptr;
	}

	void Modifier::init(Core::Renderer* renderer, Core::Scene* scene, ContentLoader* contentLoader)
	{
		_renderer = renderer;
		_scene = scene;
        _contentLoader = contentLoader;
	}
}