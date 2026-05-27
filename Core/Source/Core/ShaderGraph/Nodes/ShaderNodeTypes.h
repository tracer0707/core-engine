#pragma once

enum class ShaderNodeType
{
	None = 0,

	Int32 = 1,
	Float = 2,
	Vec2 = 3,
	Vec3 = 4,
	Vec4 = 5,
	Mat3 = 6,
	Mat4 = 7,
	Sampler2D = 8,
	SamplerCube = 9,

	Position = 10,
	Normal = 11,
	Tangent = 12,
	Bitangent = 13,
	Binormal = 14,
	UV0 = 15,
	UV1 = 16,
	Color0 = 17,
	Color1 = 18,

	Add = 19,
	Subtract = 20,
	Multiply = 21,
	Divide = 22,

	FragmentOutput = 23,
	VertexOutput = 24
};