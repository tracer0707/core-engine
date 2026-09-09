#include "Program.h"

namespace Core
{
	bool Program::operator==(const Program& left)
	{
		return program == left.program;
	}

	int Program::getUniformLocation(uint64_t nameHash) const
	{
		for (int i = 0; i < static_cast<int>(uniforms.count()); ++i)
		{
			const Core::UniformInfo& uniform = uniforms.get(i);
			if (uniform.nameHash == nameHash)
			{
				return uniform.location;
			}
		}

		return -1;
	}
} // namespace Core