#include "GizmoRenderer.h"

#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Primitives.h>
#include <Core/Scene/Scene.h>
#include <Core/Components/Camera.h>
#include <Core/Components/Transform.h>

#include "Gizmo.h"
#include "../CSG/CSGBrush.h"

namespace Editor
{
	GizmoRenderer::GizmoRenderer(Core::Renderer* renderer, Core::Scene* scene)
	{
		_renderer = renderer;
		_scene = scene;
		_wireframeBuffer = _renderer->createBuffer(nullptr, 2048, nullptr, 0);
	}

	GizmoRenderer::~GizmoRenderer()
	{
		_renderer->deleteBuffer(_wireframeBuffer);
		_wireframeBuffer = nullptr;
		_scene = nullptr;
		_renderer = nullptr;
	}

	void GizmoRenderer::renderGizmo()
	{
		if (Gizmo::singleton()->getTransform() == nullptr)
		{
			return;
		}

		Core::Transformable* transform = Gizmo::singleton()->getTransform();

		if (Gizmo::singleton()->getObjectType() == Gizmo::ObjectType::SceneObject)
		{
		}
		else if (Gizmo::singleton()->getObjectType() == Gizmo::ObjectType::CSGBrush)
		{
			CSGBrush* brush = (CSGBrush*)Gizmo::singleton()->getObject();

			glm::mat4 view = _scene->getMainCamera()->getViewMatrix();
			glm::mat4 proj = _scene->getMainCamera()->getProjectionMatrix();
			glm::mat4 model = brush->getTransform()->getTransformMatrix();

			Core::List<uint32_t> inds = brush->getFlatIndices();
			Core::List<glm::vec3>& verts = brush->getVertices();

			Core::Primitives::wireMesh(_renderer, _wireframeBuffer, view, proj, model, verts, inds, Core::Color::RED,
									   Core::Primitives::WireframeMode::Polygon,
									   C_CCW | C_CULL_BACK | C_ENABLE_DEPTH_TEST | C_ENABLE_DEPTH_WRITE | C_ENABLE_CULL_FACE | C_DEPTH_LEQUAL);
		}
	}
} // namespace Editor