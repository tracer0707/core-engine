#include "CSGModifier.h"

#include <System/InputManager.h>
#include <Scene/Scene.h>
#include <Components/Camera.h>
#include <Renderer/Renderer.h>
#include <Renderer/Primitives.h>
#include <Renderer/Color.h>

#include "../Windows/WindowManager.h"
#include "../Windows/HierarchyWindow.h"

#include "../../CSG/CSGModel.h"
#include "../../CSG/CSGBrush.h"

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

		_hierarchyWindow = (HierarchyWindow*)Editor::WindowManager::singleton()->getWindow(HierarchyWindow::NAME);
	}

	void CSGModifier::addCSGModel()
	{
		_currentCSGModel = new CSGModel();
		_csgModels.add(_currentCSGModel);

		assert(_hierarchyWindow != nullptr && "HierarchyWindow is null");
		_hierarchyWindow->rebuild();
	}

	void CSGModifier::update()
	{
		
	}

	void CSGModifier::render()
	{
		if (_currentCSGModel == nullptr || _editMode != EditMode::AddBrush) return;

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

		auto mousePos = Core::InputManager::singleton()->getMouseRelativePosition();
		glm::vec3 screenMousePos = camera->screenToWorldPoint(glm::vec3(mousePos.first, mousePos.second, 5.0f));

		_brushMtx = glm::identity<glm::mat4>();
		_brushMtx = glm::translate(_brushMtx, screenMousePos);

		Core::Color brushColor = Core::Color(0.8f, 0.0f, 0.0f, 0.8f);

		if (_brushType == BrushType::Cube)
		{
			Core::Primitives::wireCube(view, proj, _brushMtx, glm::vec3(1.0f), glm::vec3(0.5f, 0.0f, 0.5f), brushColor, flags);
		}
	}
}