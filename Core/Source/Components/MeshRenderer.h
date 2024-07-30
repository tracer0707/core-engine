#pragma once

#include "Component.h"

namespace Core
{
	class Object;
	class Mesh;
	class Camera;

	class MeshRenderer : public Component
	{
	private:
		Mesh* mesh = nullptr;

	public:
		MeshRenderer(Object* owner);
		virtual ~MeshRenderer();

		virtual UInt32 getComponentType();

		Mesh* getMesh() { return mesh; }
		void setMesh(Mesh* value) { mesh = value; }

		void render(Camera* camera) override;
	};
}