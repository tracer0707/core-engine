#pragma once

#include <string>
#include "../Shared/List.h"

namespace Core
{
	enum class UniformType
	{
		Float,
		Int,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4,
		Sampler2D,
		SamplerCube,
		Unknown
	};

	struct UniformInfo
	{
			std::string name;
			uint64_t nameHash;
			UniformType type;
			int location;
			int size;
	};

	class Program
	{
		public:
			unsigned int program = 0;
			unsigned int vertexShader = 0;
			unsigned int fragmentShader = 0;
			unsigned int geometryShader = 0;
			unsigned int computeShader = 0;

			Core::List<UniformInfo> uniforms;

			Program& operator=(const Program& left);
			bool operator==(const Program& left);
	};
} // namespace Core