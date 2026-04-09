#pragma once

#include "../../Shared/String.h"

namespace Core
{
	class Shader;

	class ShaderCompilerGLSL
	{
		public:
			static void compile(Shader* shader, String* outVertex, String* outFragment);
	};
} // namespace Core