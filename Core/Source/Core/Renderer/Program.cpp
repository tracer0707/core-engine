#include "Program.h"

namespace Core
{
	Program& Program::operator=(const Program& left)
	{
		program = left.program;
		vertexShader = left.vertexShader;
		fragmentShader = left.fragmentShader;
		geometryShader = left.geometryShader;
		computeShader = left.computeShader;

		return *this;
	}

	bool Program::operator==(const Program& left)
	{
		return program == left.program;
	}
} // namespace Core