#include "Material.h"

#include "Shader.h"
#include "Texture.h"

namespace Core
{
	Shader* Material::_shader = nullptr;

	Material::Material()
	{
		if (_shader != nullptr)
			return;

		_shader = new Shader();
		_shader->loadFromString("#version 400\n"
			"in vec3 position;"
			"in vec2 uv0;"
			"out vec2 f_uv0;"
			"uniform mat4 u_viewMtx;"
			"uniform mat4 u_projMtx;"
			"uniform mat4 u_modelMtx;"
			"void main() {"
			"  f_uv0 = uv0;"
			"  gl_Position = u_projMtx * u_viewMtx * u_modelMtx * vec4(position, 1.0);"
			"}",
			"#version 400\n"
			"in vec2 f_uv0;"
			"out vec4 frag_colour;"
			"uniform sampler2D u_diffuseTex;"
			"void main() {"
			"  vec4 color = texture2D(u_diffuseTex, f_uv0);"
			"  frag_colour = vec4(color.rgb, 1.0);"
			"}");

		Texture* texture = Texture::loadFromFile("C:/Dev/core-engine/x64/Release/1.jpg", TextureFormat::BC7);
		setTexture(texture);
	}

	Material::~Material()
	{
		if (isLoaded())
			unload();
	}

	void Material::bind()
	{
		if (_shader != nullptr)
			_shader->bind();

		if (_texture != nullptr)
			_texture->bind("u_diffuseTex", 0);
	}
}