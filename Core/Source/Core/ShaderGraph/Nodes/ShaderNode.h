#pragma once

#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "../../Shared/String.h"

#include "ShaderNodeTypes.h"

namespace Core
{
	class ShaderGraph;

	class ShaderNode
	{
			friend class ShaderGraph;

		protected:
			String _varName;
			int _intValue = 0;
			float _floatValue = 0.0f;
			glm::vec2 _vec2Value = glm::vec2(0.0f);
			glm::vec3 _vec3Value = glm::vec3(0.0f);
			glm::vec4 _vec4Value = glm::vec4(0.0f);

			ShaderNode(const String& varName) { _varName = varName; }
			virtual ~ShaderNode() = default;

		public:
			virtual ShaderNodeType getType() const = 0;

			const String& getVarName() const { return _varName; }
			void setVarName(const String& varName) { _varName = varName; }

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