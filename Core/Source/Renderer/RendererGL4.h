#pragma once

#include "Renderer.h"

namespace Core
{
	class RendererGL4 : public Renderer
	{
	private:

	public:
		const Program createProgram(UString vertexSrc, UString fragmentSrc);
		const void deleteProgram(const Program& programId);
		const void bindProgram(const Program& programId);

		const Buffer createBuffer(real* vertexArray, uint vertexArraySize, uint* indexArray, uint indexArraySize);
		const void deleteBuffer(const Buffer& buffer);
		const void bindBuffer(const Buffer& buffer);
		const void drawBuffer(const Buffer& buffer);
	};	
}