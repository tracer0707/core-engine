#include "Material.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/Program.h"
#include "../Content/Texture2D.h"

namespace Core
{
	Material::Material(Renderer* renderer) : Content()
	{
		_renderer = renderer;
		_program = renderer->getDefaultProgram();
	}

	Material::~Material()
	{
		_program = nullptr;
		_renderer = nullptr;
	}

	int Material::getInt(uint64_t name) const
	{
		auto it = _intValues.find(name);
		return it != _intValues.end() ? it->second : 0;
	}

	float Material::getFloat(uint64_t name) const
	{
		auto it = _floatValues.find(name);
		return it != _floatValues.end() ? it->second : 0.0f;
	}

	glm::vec2 Material::getVec2(uint64_t name) const
	{
		auto it = _vec2Values.find(name);
		return it != _vec2Values.end() ? it->second : glm::vec2(0.0f);
	}

	glm::vec3 Material::getVec3(uint64_t name) const
	{
		auto it = _vec3Values.find(name);
		return it != _vec3Values.end() ? it->second : glm::vec3(0.0f);
	}

	glm::vec4 Material::getVec4(uint64_t name) const
	{
		auto it = _vec4Values.find(name);
		return it != _vec4Values.end() ? it->second : glm::vec4(0.0f);
	}

	glm::mat3 Material::getMat3(uint64_t name) const
	{
		auto it = _mat3Values.find(name);
		return it != _mat3Values.end() ? it->second : glm::mat3(1.0f);
	}

	glm::mat4 Material::getMat4(uint64_t name) const
	{
		auto it = _mat4Values.find(name);
		return it != _mat4Values.end() ? it->second : glm::mat4(1.0f);
	}

	Texture2D* Material::getTexture2D(uint64_t name) const
	{
		auto it = _texture2dValues.find(name);
		return it != _texture2dValues.end() ? it->second : nullptr;
	}

	void Material::bind()
	{
		_renderer->bindProgram(_program);

		if (_program == nullptr) return;

		int texture2dSlot = 0;
		for (const auto& uniform : _program->uniforms)
		{
			if (uniform.type == UniformType::Int)
			{
				_renderer->setUniform(uniform.location, getInt(uniform.nameHash));
			}
			else if (uniform.type == UniformType::Float)
			{
				_renderer->setUniform(uniform.location, getFloat(uniform.nameHash));
			}
			else if (uniform.type == UniformType::Vec2)
			{
				_renderer->setUniform(uniform.location, getVec2(uniform.nameHash));
			}
			else if (uniform.type == UniformType::Vec3)
			{
				_renderer->setUniform(uniform.location, getVec3(uniform.nameHash));
			}
			else if (uniform.type == UniformType::Vec4)
			{
				_renderer->setUniform(uniform.location, getVec4(uniform.nameHash));
			}
			else if (uniform.type == UniformType::Mat3)
			{
				_renderer->setUniform(uniform.location, getMat3(uniform.nameHash));
			}
			else if (uniform.type == UniformType::Mat4)
			{
				_renderer->setUniform(uniform.location, getMat4(uniform.nameHash));
			}
			else if (uniform.type == UniformType::Sampler2D)
			{
				Texture2D* texture = getTexture2D(uniform.nameHash);
				if (texture != nullptr)
				{
					_renderer->bindTexture(texture->getNativeId(), texture2dSlot);
					_renderer->setUniform(uniform.location, texture2dSlot);
					texture2dSlot++;
				}
			}
		}
	}
} // namespace Core