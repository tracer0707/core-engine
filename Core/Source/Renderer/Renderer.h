#pragma once

#include "../Config.h"
#include <vector>
#include <map>
#include <glm/mat4x4.hpp>

#include "../Shared/String.h"
#include "VertexBuffer.h"

#include <GL/glew.h>

namespace Core
{
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

	protected:
		std::vector<Program> shaderPrograms;

		Program& currentProgram = defaultProgram;

		int width = 0;
		int height = 0;

	public:
		Renderer() = default;
		static Renderer* singleton();

		static void init();

		virtual const void setViewportSize(int w, int h) = 0;

		virtual const Program createProgram(UString vertexSrc, UString fragmentSrc) = 0;
		virtual const void deleteProgram(const Program& program) = 0;
		virtual const void bindProgram(const Program& program) = 0;

		virtual const VertexBuffer* createBuffer(Vertex* vertexArray, UInt32 vertexArraySize, UInt32* indexArray, UInt32 indexArraySize) = 0;
		virtual const void deleteBuffer(const VertexBuffer* buffer) = 0;
		virtual const void bindBuffer(const VertexBuffer* buffer) = 0;
		virtual const void drawBuffer(const VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model) = 0;

		virtual const UInt32 createTexture(unsigned char* data, UInt32 width, UInt32 height, UInt32 size, GLenum format) = 0;
		virtual const void bindTexture(UInt32 id, const char* name, UInt32 slot) = 0;
		virtual const void deleteTexture(UInt32 id) = 0;
	};
}