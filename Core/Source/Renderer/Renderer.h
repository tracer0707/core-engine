#pragma once

#include <vector>
#include <map>

namespace Core
{
	class Renderer
	{
	protected:
		struct Program
		{
			unsigned int program = 0;
			unsigned int vertexShader = 0;
			unsigned int fragmentShader = 0;
			unsigned int geometryShader = 0;
			unsigned int computeShader = 0;
		};

		std::vector<Program> shaderPrograms;

	public:
		virtual const unsigned int createProgram(const char* vertexSrc, const char* fragmentSrc) = 0;
		virtual const void deleteProgram(unsigned int programId) = 0;
	};
}