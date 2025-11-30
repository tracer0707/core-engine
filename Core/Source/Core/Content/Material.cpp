#include "Material.h"

#include "Shader.h"
#include "Texture.h"

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

	void Material::bind()
	{
		if (_shader != nullptr)
			_shader->bind();

		if (_texture != nullptr)
			_texture->bind("u_diffuseTex", 0);
	}
}