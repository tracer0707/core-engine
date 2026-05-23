#pragma once

#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "../../Shared/String.h"

namespace Core
{
	enum class ShaderNodeType
	{
		None = 0,

		// Data types - 1..99
		Int32 = 1,
		Float = 2,
		Vec2 = 3,
		Vec3 = 4,
		Vec4 = 5,
		Mat3 = 6,
		Mat4 = 7,
		Sampler2D = 8,
		SamplerCube = 9,

		// Attributes - 100...199
		Position = 100,
		Normal = 101,
		Tangent = 102,
		Bitangent = 103,
		Binormal = 104,
		UV0 = 105,
		UV1 = 106,
		Color0 = 107,
		Color1 = 108,

		// Functions - 200..999
		Add = 200,
		Subtract = 201,
		Multiply = 202,
		Divide = 203,
	};

	enum class ShaderParameterName
	{
		None = 0,
		Position = 1,
		UV = 2,
		Color = 3,
		Result = 4,
	};

	class ShaderNode
	{
		private:
			ShaderNodeType _type = ShaderNodeType::None;

			int _intValue = 0;
			float _floatValue = 0.0f;
			glm::vec2 _vec2Value = glm::vec2(0.0f);
			glm::vec3 _vec3Value = glm::vec3(0.0f);
			glm::vec4 _vec4Value = glm::vec4(0.0f);

			std::map<ShaderParameterName, ShaderNode*> _inputs;
			std::map<ShaderParameterName, ShaderNode*> _outputs;

		public:
			ShaderNode() = default;
			virtual ~ShaderNode() = default;

			ShaderNodeType getType() const { return _type; }
			void setType(ShaderNodeType type) { _type = type; }

			int getIntValue() const { return _intValue; }
			float getFloatValue() const { return _floatValue; }
			glm::vec2 getVec2Value() const { return _vec2Value; }
			glm::vec3 getVec3Value() const { return _vec3Value; }
			glm::vec4 getVec4Value() const { return _vec4Value; }

			void setValue(int value) { _intValue = value; }
			void setValue(float value) { _floatValue = value; }
			void setValue(glm::vec2 value) { _vec2Value = value; }
			void setValue(glm::vec3 value) { _vec3Value = value; }
			void setValue(glm::vec4 value) { _vec4Value = value; }
	};
} // namespace Core