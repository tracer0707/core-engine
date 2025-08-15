#pragma

namespace Core
{
	class Program
	{
	public:
		unsigned int program = 0;
		unsigned int vertexShader = 0;
		unsigned int fragmentShader = 0;
		unsigned int geometryShader = 0;
		unsigned int computeShader = 0;

		const Program& operator =(const Program& left);
		const bool operator ==(const Program& left);
	};
}