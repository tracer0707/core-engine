#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Editor
{
	class Gizmo
	{
	public:
		enum class TransformSpace
		{
			Local,
			World,
		};

		enum class TransformMode
		{
			Translate,
			Rotate,
			Scale
		};

	private:
		TransformSpace transformSpace = TransformSpace::World;
		TransformMode transformMode = TransformMode::Translate;

	public:
		TransformSpace getTransformSpace() { return transformSpace; }
		void setTransformSpace(TransformSpace value) { transformSpace = value; }

		TransformMode getTransformMode() { return transformMode; }
		void setTransformMode(TransformMode value) { transformMode = value; }

		void manipulate(glm::mat4& view, glm::mat4& proj, glm::mat4& model);
	};
}