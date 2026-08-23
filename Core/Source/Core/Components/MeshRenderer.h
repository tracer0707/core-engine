#pragma once

#include "Component.h"
#include "../Shared/List.h"
#include "../Math/AxisAlignedBox.h"

namespace Core
{
	class Object;
	class Mesh;
	class Camera;
	class Renderer;
	class Material;

	class MeshRenderer : public Component
	{
	private:
		Renderer* _renderer = nullptr;
		Mesh* mesh = nullptr;

		List<Material*> _materials;

	public:
		MeshRenderer(Object* owner, Renderer* renderer);
		virtual ~MeshRenderer();

		virtual ComponentType getComponentType() const { return ComponentType::MeshRenderer; }

		AxisAlignedBox getWorldBoundingBox();

		List<Material*>& getMaterials() { return _materials; }

		Mesh* getMesh() { return mesh; }
		void setMesh(Mesh* value);

		void render(Camera* camera) override;
	};
}