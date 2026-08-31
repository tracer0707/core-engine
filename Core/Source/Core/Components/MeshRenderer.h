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
		int getMaterialCount() const { return _materials.count(); }
		Material* getMaterial(int index) { return _materials.get(index); }
		void setMaterial(int index, Material* value) { _materials.set(index, value); }

		Mesh* getMesh() { return mesh; }
		void setMesh(Mesh* value);

		void render(glm::mat4& view, glm::mat4& proj) override;
	};
}