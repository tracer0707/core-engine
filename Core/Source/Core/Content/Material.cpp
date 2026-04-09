#include "Material.h"

#include "ContentTypes.h"

#include "../ShaderGraph/Shader.h"

namespace Core
{
	Material::Material(Renderer* renderer) : Content()
	{
		_renderer = renderer;
	}

	Material::~Material()
	{
		_shader = nullptr;
		_renderer = nullptr;
	}

	int Material::getContentType()
	{
		return CONTENT_TYPE_MATERIAL;
	}

	void Material::bind()
	{
		if (_shader != nullptr) _shader->bind();
	}
} // namespace Core