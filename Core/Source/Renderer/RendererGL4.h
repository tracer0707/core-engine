#pragma once

#include <glm/mat4x4.hpp>

#include "Renderer.h"

namespace Core
{
	class RendererGL4 : public Renderer
	{
	friend class Renderer;

	protected:
		RendererGL4(DeviceContext* ctx);

	public:
		virtual const void setViewportSize(int w, int h);

		const Program createProgram(UString vertexSrc, UString fragmentSrc);
		const void deleteProgram(const Program& programId);
		const void bindProgram(const Program& programId);

		const VertexBuffer* createBuffer(Vertex* vertexArray, UInt32 vertexArraySize, UInt32* indexArray, UInt32 indexArraySize);
		const void deleteBuffer(const VertexBuffer* buffer);
		const void bindBuffer(const VertexBuffer* buffer);
		const void drawBuffer(const VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model);

		virtual const UInt32 createTexture(unsigned char* data, UInt32 width, UInt32 height, UInt32 size, GLenum format);
		virtual const void bindTexture(UInt32 id, const char* name, UInt32 slot);
		virtual const void deleteTexture(UInt32 id);
	};	
}