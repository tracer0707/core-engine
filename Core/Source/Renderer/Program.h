#pragma

#include "../Config.h"

namespace Core
{
	class Program
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
}