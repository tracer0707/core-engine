#pragma once

#include "Renderer.h"

namespace Core
{
	class RendererGL4 : public Renderer
	{
	private:

	public:
		const unsigned int createProgram(UString vertexSrc, UString fragmentSrc);
		const void deleteProgram(unsigned int programId);
		const void bindProgram(unsigned int programId);

		static RendererGL4* singleton();
	};	
}