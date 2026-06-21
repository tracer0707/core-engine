#pragma once

#include <string>
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

			virtual void makeCurrent();
			virtual void swapBuffers();
			virtual void processEvents(void* event);

		public:
			virtual void setViewportSize(int w, int h);
			virtual void beginUI();
			virtual void endUI();

			virtual Program* createProgram(String name, String vertexSrc, String fragmentSrc);
			virtual void deleteProgram(Program* program);
			virtual void bindProgram(Program* program);
			virtual std::string checkProgramErrors(unsigned int program);

			virtual VertexBuffer* createBuffer(unsigned int maxVertexSize, unsigned int maxIndexSize);
			virtual VertexBuffer* createBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray,
											   unsigned int indexArraySize);

			virtual void deleteBuffer(VertexBuffer* buffer);

			virtual void drawBuffer(VertexBuffer* buffer, PrimitiveType primitiveType, unsigned int flags, glm::mat4& view, glm::mat4& proj,
									glm::mat4& model);

			virtual void updateBuffer(VertexBuffer* buffer, Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray,
									  unsigned int indexArraySize);

			virtual const FrameBuffer* createFrameBuffer(unsigned int width, unsigned int height);
			virtual void deleteFrameBuffer(const FrameBuffer* buffer);
			virtual void bindFrameBuffer(const FrameBuffer* buffer);

			virtual const unsigned int createTexture(unsigned char* data, unsigned int width, unsigned int height, unsigned int size,
													 TextureFormat format);
			virtual void bindTexture(unsigned int id, unsigned int slot);
			virtual void deleteTexture(unsigned int id);

			virtual void setUniform(int location, int value);
			virtual void setUniform(int location, float value);
			virtual void setUniform(int location, glm::vec2 value);
			virtual void setUniform(int location, glm::vec3 value);
			virtual void setUniform(int location, glm::vec4 value);
			virtual void setUniform(int location, glm::mat3 value);
			virtual void setUniform(int location, glm::mat4 value);

			virtual void clear(unsigned int flags, Color color);
	};
} // namespace Core