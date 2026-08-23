#include "MeshRenderer.h"

#include "Transform.h"

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
		mesh = nullptr;
	}

	AxisAlignedBox MeshRenderer::getWorldBoundingBox()
	{
		if (mesh == nullptr) return AxisAlignedBox::BOX_NULL;

		Transform* transform = _owner->findComponent<Transform*>();

		AxisAlignedBox aab = mesh->getBoundingBox();
		aab.transform(transform->getTransformMatrix());

		return aab;
	}

	void MeshRenderer::setMesh(Mesh* value)
	{
		mesh = value;
		
		_materials.clear();
		for (int i = 0; i < mesh->getSubMeshCount(); ++i)
		{
			_materials.add(nullptr);
		}
	}

	void MeshRenderer::render(Camera* camera)
	{
		if (mesh == nullptr) return;

		Transform* transform = _owner->findComponent<Transform*>();

		glm::mat4 view = camera->getViewMatrix();
		glm::mat4 proj = camera->getProjectionMatrix();
		glm::mat4 model = transform->getTransformMatrix();

		_renderer->bindBuffer(mesh->getVertexBuffer(),
							  C_CCW | C_CULL_BACK | C_ENABLE_DEPTH_TEST | C_ENABLE_DEPTH_WRITE | C_ENABLE_CULL_FACE | C_DEPTH_LEQUAL, view, proj,
							  model);

		for (int i = 0; i < mesh->getSubMeshCount(); ++i)
		{
			SubMesh& subMesh = mesh->getSubMesh(i);
			Material* material = _materials[i];

			if (material != nullptr)
			{
				material->bind();
			}
			else
			{
				_renderer->bindProgram(nullptr);
			}

			if (mesh->getVertexBuffer()->indexArraySize > 0)
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