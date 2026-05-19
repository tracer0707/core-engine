#include "Material.h"

#include "ContentTypes.h"

#include "../ShaderGraph/ShaderGraph.h"

namespace Core
{
	Material::Material(Renderer* renderer) : Content()
	{
		_renderer = renderer;
	}

	Material::~Material()
	{
		_shaderGraph = nullptr;
		_renderer = nullptr;
	}

	int Material::getContentType()
	{
		return CONTENT_TYPE_MATERIAL;
	}

	void Material::bind()
	{
		if (_shaderGraph != nullptr) _shaderGraph->bind();
	}
} // namespace Core