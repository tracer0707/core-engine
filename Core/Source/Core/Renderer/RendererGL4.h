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
        void setViewportSize(int w, int h);
        void beginUI();
        void endUI();

        Program* createProgram(String vertexSrc, String fragmentSrc);
        void deleteProgram(Program* programId);
        void bindProgram(Program* programId);
        std::string checkProgramErrors(unsigned int program);

        const VertexBuffer* createBuffer(unsigned int maxVertexSize, unsigned int maxIndexSize);
        const VertexBuffer* createBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize);

        void deleteBuffer(const VertexBuffer* buffer);

        void drawBuffer(const VertexBuffer* buffer, PrimitiveType primitiveType, unsigned int flags, glm::mat4& view, glm::mat4& proj,
                        glm::mat4& model);

        virtual void updateBuffer(const VertexBuffer* buffer, Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray,
                                  unsigned int indexArraySize);

        const FrameBuffer* createFrameBuffer(unsigned int width, unsigned int height);
        void deleteFrameBuffer(const FrameBuffer* buffer);
        void bindFrameBuffer(const FrameBuffer* buffer);

        const unsigned int createTexture(unsigned char* data, unsigned int width, unsigned int height, unsigned int size, TextureFormat format);
        void bindTexture(unsigned int id, const char* name, unsigned int slot);
        void deleteTexture(unsigned int id);

        void clear(unsigned int flags, Color color);
    };
} // namespace Core