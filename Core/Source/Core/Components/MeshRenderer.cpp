#include "MeshRenderer.h"

#include "../Interface/Transform.h"

#include "../Scene/Object.h"
#include "../Content/Material.h"
#include "../Content/Mesh.h"
#include "../Components/Camera.h"
#include "../Renderer/Renderer.h"

namespace Core
{
	MeshRenderer::MeshRenderer(Object* owner, Renderer* renderer) : Component(owner)
	{
		_renderer = renderer;
	}

	MeshRenderer::~MeshRenderer()
	{
		_mesh = nullptr;
	}

	AxisAlignedBox MeshRenderer::getWorldBoundingBox()
	{
		if (_mesh == nullptr) return AxisAlignedBox::BOX_NULL;

		Transform* transform = _owner->getTransform();

		AxisAlignedBox aab = _mesh->getBoundingBox();
		aab.transform(transform->getTransformMatrix());

		return aab;
	}

	void MeshRenderer::setMesh(Mesh* value)
	{
		_mesh = value;
		_materials.clear();

		if (_mesh == nullptr)
		{
			return;
		}
		
		for (int i = 0; i < _mesh->getSubMeshCount(); ++i)
		{
			_materials.add(nullptr);
		}
	}

	void MeshRenderer::render(glm::mat4& view, glm::mat4& proj)
	{
		if (_mesh == nullptr) return;

		Transform* transform = _owner->getTransform();

		glm::mat4 model = transform->getTransformMatrix();

		_renderer->bindBuffer(_mesh->getVertexBuffer(), C_CCW | C_CULL_BACK | C_ENABLE_DEPTH_TEST | C_ENABLE_DEPTH_WRITE | C_ENABLE_CULL_FACE | C_DEPTH_LEQUAL);

		for (int i = 0; i < _mesh->getSubMeshCount(); ++i)
		{
			SubMesh& subMesh = _mesh->getSubMesh(i);
			Material* material = _materials[i];

			if (material != nullptr)
			{
				material->bind();
			}
			else
			{
				_renderer->bindProgram(nullptr);
			}

			_renderer->setTransform(view, proj, model);

			if (_mesh->getVertexBuffer()->getIndexArraySize() > 0)
			{
				_renderer->drawBufferIndexed(PrimitiveType::Triangle, subMesh.getIndexOffset(), subMesh.getIndexCount());
			}
			else
			{
				_renderer->drawBufferArray(PrimitiveType::Triangle, subMesh.getIndexOffset(), subMesh.getIndexCount());
			}
		}
	}
} // namespace Core