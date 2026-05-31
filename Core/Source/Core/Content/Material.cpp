#include "Material.h"

#include "ContentTypes.h"

#include "../Renderer/Renderer.h"

namespace Core
{
	Material::Material(Renderer* renderer) : Content()
	{
		_renderer = renderer;
	}

	Material::~Material()
	{
		_program = nullptr;
		_renderer = nullptr;
	}

	int Material::getContentType()
	{
		return CONTENT_TYPE_MATERIAL;
	}

	void Material::bind()
	{
		_renderer->bindProgram(_program);
	}
} // namespace Core