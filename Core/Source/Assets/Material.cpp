#include "Material.h"

#include "Shader.h"
#include "Texture.h"

namespace Core
{
	std::map<Renderer*, Material*> Material::defaultMaterials;

	Material::Material()
	{
		_shader = new Shader();
		_shader->loadFromString("#version 400\n"
			"in vec3 position;"
			"in vec2 uv0;"
			"in vec4 color0;"
			"out vec2 f_uv0;"
			"out vec4 f_color0;"
			"uniform mat4 u_viewMtx;"
			"uniform mat4 u_projMtx;"
			"uniform mat4 u_modelMtx;"
			"void main() {"
			"  f_uv0 = uv0;"
			"  f_color0 = color0;"
			"  gl_Position = u_projMtx * u_viewMtx * u_modelMtx * vec4(position, 1.0);"
			"}",
			"#version 400\n"
			"in vec2 f_uv0;"
			"in vec4 f_color0;"
			"out vec4 frag_colour;"
			"uniform sampler2D u_diffuseTex;"
			"void main() {"
			"  vec4 out_color = texture2D(u_diffuseTex, f_uv0);"
			"  frag_colour = vec4(f_color0.rgb, 1.0);"
			"}");

		//Texture* texture = Texture::loadFromFile("D:/Dev/C++/core-engine/x64/Release/Test Project/diffuse.jpg", TextureFormat::BC7);
		//setTexture(texture);
	}

	Material::~Material()
	{
		if (_shader != nullptr)
		{
			delete _shader;
			_shader = nullptr;
		}
	}

	Material* Material::getDefaultMaterial()
	{
		Renderer* renderer = Renderer::current();
		if (renderer == nullptr) return nullptr;

		auto it = defaultMaterials.find(renderer);
		if (it != defaultMaterials.end())
		{
			return it->second;
		}

		Material* defaultMaterial = new Material();
		defaultMaterials[renderer] = defaultMaterial;
		return defaultMaterial;
	}

	void Material::destroyDefaultMaterial(Renderer* renderer)
	{
		auto it = defaultMaterials.find(renderer);
		if (it != defaultMaterials.end())
		{
			delete it->second;
			defaultMaterials.erase(it);
		}
	}

	void Material::bind()
	{
		if (_shader != nullptr)
			_shader->bind();

		if (_texture != nullptr)
			_texture->bind("u_diffuseTex", 0);
	}
}