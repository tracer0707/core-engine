#include "CSGModifier.h"

#include <System/InputManager.h>
#include <Scene/Scene.h>
#include <Components/Camera.h>
#include <Renderer/Renderer.h>
#include <Renderer/Primitives.h>
#include <Renderer/Color.h>

namespace Editor
{
	UString CSGModifier::NAME = "CSG";

	CSGModifier::CSGModifier() : Modifier(NAME)
	{
	}

	CSGModifier::~CSGModifier()
	{
	}

	void CSGModifier::init(Core::Scene* scene)
	{
		Modifier::init(scene);
	}

	void CSGModifier::update()
	{
	}

	void CSGModifier::render()
	{
		Core::Camera* camera = _scene->getMainCamera();

		if (camera == nullptr) return;

		UInt32 flags = C_CCW
			| C_CULL_BACK
			| C_ENABLE_DEPTH_TEST
			| C_ENABLE_DEPTH_WRITE
			| C_ENABLE_CULL_FACE
			| C_DEPTH_LEQUAL;

		glm::mat4 view = camera->getViewMatrix();
		glm::mat4 proj = camera->getProjectionMatrix();
		glm::mat4 mtx = glm::identity<glm::mat4>();

		auto mousePos = Core::InputManager::singleton()->getMouseRelativePosition();
		glm::vec3 cursorPos = camera->screenToWorldPoint(glm::vec3(mousePos.first, mousePos.second, 3.0f));

		Core::Color brushColor = Core::Color(0.8f, 0.0f, 0.0f, 0.8f);

		Core::List<Core::Vertex> verts;

		Core::Vertex v1, v2;

		v1.make(cursorPos, {0, 0}, brushColor);
		v2.make(cursorPos + glm::vec3(0.0f, 1.0f, 0.0f), { 0, 0 }, brushColor);

		verts.add(v1);
		verts.add(v2);

		if (_brushType == BrushType::Cube)
		{
			Core::Primitives::lines(view, proj, mtx, verts.ptr(), verts.count(), flags);
		}
	}
}