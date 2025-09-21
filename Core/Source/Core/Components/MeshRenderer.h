#pragma once

#include "Component.h"
#include "../Math/AxisAlignedBox.h"

namespace Core
{
	class Object;
	class Mesh;
	class Camera;
	class Renderer;

	class MeshRenderer : public Component
	{
	private:
		Renderer* _renderer = nullptr;
		Mesh* mesh = nullptr;

	public:
		MeshRenderer(Object* owner, Renderer* renderer);
		virtual ~MeshRenderer();

		virtual unsigned int getComponentType();

		AxisAlignedBox getWorldBoundingBox();

		Mesh* getMesh() { return mesh; }
		void setMesh(Mesh* value) { mesh = value; }

		void render(Camera* camera) override;
	};
}