#pragma once

#include "../Config.h"
#include <vector>
#include <map>

#include "../Shared/String.h"

namespace Core
{
	class Camera;

	struct Program
	{
	public:
		uint program = 0;
		uint vertexShader = 0;
		uint fragmentShader = 0;
		uint geometryShader = 0;
		uint computeShader = 0;

		const Program& operator =(const Program& left);
		const bool operator ==(const Program& left);
	};

	struct Buffer
	{
	public:
		uint vbo = 0;
		uint ibo = 0;
		real* vertexArray = nullptr;
		uint vertexArraySize = 0;
		uint* indexArray = nullptr;
		uint indexArraySize = 0;

		const Buffer& operator =(const Buffer& left);
		const bool operator ==(const Buffer& left);
	};

	class Renderer
	{
	protected:
		std::vector<Program> shaderPrograms;
		std::vector<Buffer> buffers;

	public:
		Renderer() = default;
		static Renderer* singleton();

		static void init();

		virtual const Program createProgram(UString vertexSrc, UString fragmentSrc) = 0;
		virtual const void deleteProgram(const Program& program) = 0;
		virtual const void bindProgram(const Program& program) = 0;

		virtual const Buffer createBuffer(real* vertexArray, uint vertexArraySize, uint* indexArray, uint indexArraySize) = 0;
		virtual const void deleteBuffer(const Buffer& buffer) = 0;
		virtual const void bindBuffer(const Buffer& buffer) = 0;
		virtual const void drawBuffer(const Buffer& buffer) = 0;

		virtual const void frame(Camera* camera);
	};
}