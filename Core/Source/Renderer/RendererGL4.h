#pragma once

#include "Renderer.h"

namespace Core
{
	class RendererGL4 : public Renderer
	{
		const unsigned int createProgram(const char* vertexSrc, const char* fragmentSrc);
		const void deleteProgram(unsigned int programId);
	};
}