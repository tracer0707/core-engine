#pragma once

#include <string>
#include <vector>
#include <map>
#include <glm/mat4x4.hpp>

#include "../Shared/String.h"
#include "VertexBuffer.h"
#include "FrameBuffer.h"
#include "Color.h"
#include "TextureFormat.h"

// Render flags
#define C_CW 1 << 0
#define C_CCW 1 << 1
#define C_CULL_BACK 1 << 2
#define C_CULL_FRONT 1 << 3
#define C_ENABLE_DEPTH_TEST 1 << 4
#define C_ENABLE_DEPTH_WRITE 1 << 5
#define C_ENABLE_CULL_FACE 1 << 6
#define C_DEPTH_ALWAYS 1 << 7
#define C_DEPTH_NEVER 1 << 8
#define C_DEPTH_LEQUAL 1 << 9
#define C_DEPTH_GEQUAL 1 << 10
#define C_DEPTH_LESS 1 << 11
#define C_DEPTH_GREATER 1 << 12
#define C_DEPTH_EQUAL 1 << 13
#define C_DEPTH_NOTEQUAL 1 << 14

// Clear flags
#define C_CLEAR_COLOR 1 << 0
#define C_CLEAR_DEPTH 1 << 1

struct ImGuiContext;

namespace Core
{
    class DeviceContext;
    class Program;

    enum class PrimitiveType
    {
        Triangle,
        Line
    };

    class Renderer
    {
        friend class Window;

      private:
        static Renderer* init(void* windowCtx);
        static void destroy(Renderer* renderer);

      protected:
        Renderer(void* windowCtx);
        virtual ~Renderer();

        void* _windowCtx = nullptr;
        void* _renderCtx = nullptr;
        ImGuiContext* _imguiCtx = nullptr;

        virtual void makeCurrent() = 0;
        virtual void swapBuffers() = 0;
        virtual void processEvents(void* event) = 0;

        Program* _defaultProgram = nullptr;
        std::vector<Program*> _shaderPrograms;

        Program* _currentProgram = nullptr;

        unsigned int _width = 0;
        unsigned int _height = 0;

      public:
        unsigned int getWidth() { return _width; }
        unsigned int getHeight() { return _height; }

        virtual void setViewportSize(int w, int h) = 0;
        virtual void beginUI() = 0;
        virtual void endUI() = 0;

        virtual Program* createProgram(String vertexSrc, String fragmentSrc) = 0;
        virtual void deleteProgram(Program* program) = 0;
        virtual void bindProgram(Program* program) = 0;
        virtual std::string checkProgramErrors(unsigned int program) = 0;

        // Create dynamic buffer
        virtual VertexBuffer* createBuffer(unsigned int maxVertexSize, unsigned int maxIndexSize) = 0;

        // Create static buffer
        virtual VertexBuffer* createBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray,
                                           unsigned int indexArraySize) = 0;

        virtual void deleteBuffer(VertexBuffer* buffer) = 0;

        virtual void drawBuffer(VertexBuffer* buffer, PrimitiveType primitiveType, unsigned int flags, glm::mat4& view, glm::mat4& proj,
                                glm::mat4& model) = 0;

        virtual void updateBuffer(VertexBuffer* buffer, Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray,
                                  unsigned int indexArraySize) = 0;

        virtual const FrameBuffer* createFrameBuffer(unsigned int width, unsigned int height) = 0;
        virtual void deleteFrameBuffer(const FrameBuffer* buffer) = 0;
        virtual void bindFrameBuffer(const FrameBuffer* buffer) = 0;

        virtual const unsigned int createTexture(unsigned char* data, unsigned int width, unsigned int height, unsigned int size,
                                                 TextureFormat format) = 0;
        virtual void bindTexture(unsigned int id, const char* name, unsigned int slot) = 0;
        virtual void deleteTexture(unsigned int id) = 0;

        virtual void clear(unsigned int flags, Color color) = 0;
    };
} // namespace Core