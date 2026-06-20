#include "Material.h"

#include "ContentTypes.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/Program.h"
#include "../Content/Texture2D.h"

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

	int Material::getInt(std::string name) const
	{
		auto it = _intValues.find(name);
		return it != _intValues.end() ? it->second : 0;
	}

	float Material::getFloat(std::string name) const
	{
		auto it = _floatValues.find(name);
		return it != _floatValues.end() ? it->second : 0.0f;
	}

	glm::vec2 Material::getVec2(std::string name) const
	{
		auto it = _vec2Values.find(name);
		return it != _vec2Values.end() ? it->second : glm::vec2(0.0f);
	}

	glm::vec3 Material::getVec3(std::string name) const
	{
		auto it = _vec3Values.find(name);
		return it != _vec3Values.end() ? it->second : glm::vec3(0.0f);
	}

	glm::vec4 Material::getVec4(std::string name) const
	{
		auto it = _vec4Values.find(name);
		return it != _vec4Values.end() ? it->second : glm::vec4(0.0f);
	}

	glm::mat3 Material::getMat3(std::string name) const
	{
		auto it = _mat3Values.find(name);
		return it != _mat3Values.end() ? it->second : glm::mat3(1.0f);
	}

	glm::mat4 Material::getMat4(std::string name) const
	{
		auto it = _mat4Values.find(name);
		return it != _mat4Values.end() ? it->second : glm::mat4(1.0f);
	}

	Texture2D* Material::getTexture2D(std::string name) const
	{
		auto it = _textureValues.find(name);
		return it != _textureValues.end() ? it->second : nullptr;
	}

	void Material::bind()
	{
		_renderer->bindProgram(_program);

		if (_program == nullptr) return;

		int texture2dSlot = 0;
		for (const auto& uniform : _program->uniforms)
		{
			if (uniform.second.type == UniformType::Int)
			{
				_renderer->setUniform(uniform.second.location, getInt(uniform.first));
			}
			else if (uniform.second.type == UniformType::Float)
			{
				_renderer->setUniform(uniform.second.location, getFloat(uniform.first));
			}
			else if (uniform.second.type == UniformType::Vec2)
			{
				_renderer->setUniform(uniform.second.location, getVec2(uniform.first));
			}
			else if (uniform.second.type == UniformType::Vec3)
			{
				_renderer->setUniform(uniform.second.location, getVec3(uniform.first));
			}
			else if (uniform.second.type == UniformType::Vec4)
			{
				_renderer->setUniform(uniform.second.location, getVec4(uniform.first));
			}
			else if (uniform.second.type == UniformType::Mat3)
			{
				_renderer->setUniform(uniform.second.location, getMat3(uniform.first));
			}
			else if (uniform.second.type == UniformType::Mat4)
			{
				_renderer->setUniform(uniform.second.location, getMat4(uniform.first));
			}
			else if (uniform.second.type == UniformType::Sampler2D)
			{
				Texture2D* texture = getTexture2D(uniform.first);
				if (texture != nullptr)
				{
					_renderer->bindTexture(texture->getNativeId(), texture2dSlot);
					_renderer->setUniform(uniform.second.location, texture2dSlot);
					texture2dSlot++;
				}
			}
		}
	}
} // namespace Core