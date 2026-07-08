#pragma once

#include <string>
#include "../Shared/List.h"
#include "../Shared/String.h"

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
			String name;
			uint64_t nameHash;
			UniformType type;
			int location;
			int size;
	};

	class Program
	{
		public:
			String name = String::Empty;

			unsigned int program = 0;
			
			uint64_t u_viewMtxLocation = 0;
			uint64_t u_projMtxLocation = 0;
			uint64_t u_modelMtxLocation = 0;

			Core::List<UniformInfo> uniforms;

			bool operator==(const Program& left);
	};
} // namespace Core