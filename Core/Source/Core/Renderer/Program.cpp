#include "Program.h"

namespace Core
{
	const Program& Program::operator=(const Program& left)
	{
		program = left.program;
		vertexShader = left.vertexShader;
		fragmentShader = left.fragmentShader;
		geometryShader = left.geometryShader;
		computeShader = left.computeShader;

		return *this;
	}

	const bool Program::operator==(const Program& left)
	{
		return program == left.program;
	}
}