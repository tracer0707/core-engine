#pragma once

#include <glm/mat4x4.hpp>

#include "Renderer.h"

namespace Core
{
	class RendererGL4 : public Renderer
	{
	friend class Renderer;
	protected:
		RendererGL4(void* windowCtx);
		virtual ~RendererGL4();

		virtual const void makeCurrent();
		virtual const void swapBuffers();
		virtual const void processEvents(void* event);

	public:
		const void setViewportSize(int w, int h);
		const void beginUI();
		const void endUI();
		
		const Program* createProgram(String vertexSrc, String fragmentSrc);
		const void deleteProgram(const Program* programId);
		const void bindProgram(const Program* programId);
		const char* checkProgramErrors(unsigned int program);

		const VertexBuffer* createBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize);
		const void deleteBuffer(const VertexBuffer* buffer);
		const void bindBuffer(const VertexBuffer* buffer);
		const void drawBuffer(const VertexBuffer* buffer, PrimitiveType primitiveType, unsigned int flags, glm::mat4& view, glm::mat4& proj, glm::mat4& model);

		const FrameBuffer* createFrameBuffer(unsigned int width, unsigned int height);
		const void deleteFrameBuffer(const FrameBuffer* buffer);
		const void bindFrameBuffer(const FrameBuffer* buffer);

		const unsigned int createTexture(unsigned char* data, unsigned int width, unsigned int height, unsigned int size, TextureFormat format);
		const void bindTexture(unsigned int id, const char* name, unsigned int slot);
		const void deleteTexture(unsigned int id);

		const void clear(unsigned int flags, Color color);
	};	
}