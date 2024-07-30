#pragma once

#include "../Config.h"
#include <vector>
#include <map>
#include <glm/mat4x4.hpp>

#include "../Shared/String.h"
#include "VertexBuffer.h"
#include "Color.h"

#include <GL/glew.h>

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

namespace Core
{
	class DeviceContext;

	struct Program
	{
	public:
		UInt32 program = 0;
		UInt32 vertexShader = 0;
		UInt32 fragmentShader = 0;
		UInt32 geometryShader = 0;
		UInt32 computeShader = 0;

		const Program& operator =(const Program& left);
		const bool operator ==(const Program& left);
	};

	class Renderer
	{
	private:
		Program defaultProgram = { 0, 0, 0, 0, 0 };
		DeviceContext* context = nullptr;

	protected:
		Renderer(DeviceContext* ctx);

		std::vector<Program> shaderPrograms;

		Program& currentProgram = defaultProgram;

		int width = 0;
		int height = 0;

	public:
		static Renderer* singleton();

		static void init(DeviceContext* ctx);

		const UInt32& getWidth() { return width; }
		const UInt32& getHeight() { return height; }

		virtual const void setViewportSize(int w, int h) = 0;

		virtual const Program createProgram(UString vertexSrc, UString fragmentSrc) = 0;
		virtual const void deleteProgram(const Program& program) = 0;
		virtual const void bindProgram(const Program& program) = 0;

		virtual const VertexBuffer* createBuffer(Vertex* vertexArray, UInt32 vertexArraySize, UInt32* indexArray, UInt32 indexArraySize) = 0;
		virtual const void deleteBuffer(const VertexBuffer* buffer) = 0;
		virtual const void bindBuffer(const VertexBuffer* buffer) = 0;
		virtual const void drawBuffer(const VertexBuffer* buffer, int primitiveType, UInt32 flags, glm::mat4& view, glm::mat4& proj, glm::mat4& model) = 0;

		virtual const UInt32 createTexture(unsigned char* data, UInt32 width, UInt32 height, UInt32 size, UInt32 format) = 0;
		virtual const void bindTexture(UInt32 id, const char* name, UInt32 slot) = 0;
		virtual const void deleteTexture(UInt32 id) = 0;

		virtual const void clear(UInt32 flags) = 0;
	};
}