#pragma once

#include "../../Shared/String.h"

namespace Core
{
	enum class ShaderNodeType
	{
		None = 0,

		// Data types - 1..99
		Int32 = 1,
		Float = 2,
		Vec2 = 3,
		Vec3 = 4,
		Vec4 = 5,
		Mat3 = 6,
		Mat4 = 7,
		Sampler2D = 8,
		SamplerCube = 9,

		// Functions - 100..999
		Add = 100,
		Subtract = 101,
		Multiply = 102,
		Divide = 103,
	};

	class ShaderNode
	{
		public:
			ShaderNode() = default;
			virtual ~ShaderNode() = default;

			virtual ShaderNodeType getType() const = 0;
	};
} // namespace Core