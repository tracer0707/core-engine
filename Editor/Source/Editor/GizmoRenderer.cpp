#include "GizmoRenderer.h"

#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Primitives.h>
#include <Core/Content/Scene.h>
#include <Core/Scene/Object.h>
#include <Core/Components/Camera.h>
#include <Core/Interface/Transform.h>

#include "Gizmo.h"

namespace Editor
{
	GizmoRenderer::GizmoRenderer(Core::Renderer* renderer)
	{
		_renderer = renderer;
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
		if (_scene == nullptr || Gizmo::singleton()->getTransform() == nullptr)
		{
			return;
		}

		Core::Transform* transform = Gizmo::singleton()->getTransform();

		if (Gizmo::singleton()->getObjectType() == Gizmo::ObjectType::SceneObject)
		{
		}
	}
} // namespace Editor