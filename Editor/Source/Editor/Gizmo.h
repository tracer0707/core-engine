#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
	class Camera;
}

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

		static Gizmo _singleton;

		glm::mat4* _model = nullptr;

	public:
		static Gizmo* singleton() { return &_singleton; }

		void setModelMatrix(glm::mat4* value) { _model = value; }
		glm::mat4* getModelMatrix() { return _model; }

		TransformSpace getTransformSpace() { return transformSpace; }
		void setTransformSpace(TransformSpace value) { transformSpace = value; }

		TransformMode getTransformMode() { return transformMode; }
		void setTransformMode(TransformMode value) { transformMode = value; }

		void update(Core::Camera* camera, float viewX, float viewY, float viewW, float viewH);
	};
}