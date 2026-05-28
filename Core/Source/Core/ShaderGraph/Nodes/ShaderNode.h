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

		private:
			String _varName;

		protected:
			ShaderNode(const String& varName) { _varName = varName; }
			virtual ~ShaderNode() = default;

		public:
			virtual ShaderNodeType getType() const = 0;

			const String& getVarName() const { return _varName; }
			void setVarName(const String& varName) { _varName = varName; }
	};
} // namespace Core