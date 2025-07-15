#pragma once

#include "../Config.h"
#include <vector>
#include <map>
#include <glm/mat4x4.hpp>

#include "../Shared/String.h"
#include "Program.h"
#include "VertexBuffer.h"
#include "FrameBuffer.h"
#include "Color.h"
#include "TextureFormat.h"

#include <GL/glew.h>

//Render flags
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

//Clear flags
#define C_CLEAR_COLOR 1 << 0
#define C_CLEAR_DEPTH 1 << 1

namespace Core
{
	class DeviceContext;

	class Renderer
	{
	private:
		DeviceContext* context = nullptr;

	protected:
		Renderer(DeviceContext* ctx);
		virtual ~Renderer();

		Program* defaultProgram = new Program();
		std::vector<Program*> shaderPrograms;

		const Program* currentProgram = nullptr;

		int width = 0;
		int height = 0;

	public:
		static Renderer* singleton();

		static void init(DeviceContext* ctx);
		static void destroy();

		const UInt32& getWidth() { return width; }
		const UInt32& getHeight() { return height; }

		virtual const void processEvents(void* event) = 0;
		virtual const void setViewportSize(int w, int h) = 0;
		virtual const void beginUI() = 0;
		virtual const void endUI() = 0;
		virtual const void swapBuffers() = 0;

		virtual const Program* createProgram(UString vertexSrc, UString fragmentSrc) = 0;
		virtual const void deleteProgram(const Program* program) = 0;
		virtual const void bindProgram(const Program* program) = 0;
		virtual const char* checkProgramErrors(UInt32 program) = 0;

		virtual const VertexBuffer* createBuffer(Vertex* vertexArray, UInt32 vertexArraySize, UInt32* indexArray, UInt32 indexArraySize) = 0;
		virtual const void deleteBuffer(const VertexBuffer* buffer) = 0;
		virtual const void bindBuffer(const VertexBuffer* buffer) = 0;
		virtual const void drawBuffer(const VertexBuffer* buffer, int primitiveType, UInt32 flags, glm::mat4& view, glm::mat4& proj, glm::mat4& model) = 0;

		virtual const FrameBuffer* createFrameBuffer(UInt32 width, UInt32 height) = 0;
		virtual const void deleteFrameBuffer(const FrameBuffer* buffer) = 0;
		virtual const void bindFrameBuffer(const FrameBuffer* buffer) = 0;

		virtual const UInt32 createTexture(unsigned char* data, UInt32 width, UInt32 height, UInt32 size, TextureFormat format) = 0;
		virtual const void bindTexture(UInt32 id, const char* name, UInt32 slot) = 0;
		virtual const void deleteTexture(UInt32 id) = 0;

		virtual const void clear(UInt32 flags, Color color) = 0;
	};
}