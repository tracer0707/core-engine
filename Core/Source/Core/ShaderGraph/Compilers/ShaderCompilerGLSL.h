#pragma once

#include "../../Shared/String.h"

namespace Core
{
	class ShaderGraph;

	class ShaderCompilerGLSL
	{
		public:
			static void compile(ShaderGraph* shaderGraph, String* outVertex, String* outFragment);
	};
} // namespace Core