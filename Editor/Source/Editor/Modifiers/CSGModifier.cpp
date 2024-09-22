#include "CSGModifier.h"

#include <Scene/Scene.h>
#include <System/InputManager.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <Renderer/Renderer.h>
#include <Renderer/Primitives.h>
#include <Renderer/Color.h>

#include "../Windows/WindowManager.h"
#include "../Windows/HierarchyWindow.h"
#include "../Windows/CSGObjectWindow.h"
#include "../Windows/CSGEditWindow.h"

#include "../../CSG/CSGModel.h"
#include "../../CSG/CSGBrush.h"
#include "../../CSG/CSGBrushCube.h"

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
		_csgObjectWindow = (CSGObjectWindow*)Editor::WindowManager::singleton()->getWindow(CSGObjectWindow::NAME);
		_csgEditWindow = (CSGEditWindow*)Editor::WindowManager::singleton()->getWindow(CSGEditWindow::NAME);

		Core::InputManager::singleton()->subscribeMouseDownEvent([=] (Core::InputManager::MouseButton mb, int x, int y)
		{
			if (_editMode == EditMode::AddBrush && mb == Core::InputManager::MouseButton::MBE_LEFT)
			{
				if (_currentCSGModel == nullptr) return;
				addCSGBrush();
			}
		});
	}

	void CSGModifier::addCSGModel()
	{
		_currentCSGModel = new CSGModel();
		_currentCSGModel->setName("CSG Model");
		_scene->addObject(_currentCSGModel->getObject());
		_csgModels.add(_currentCSGModel);

		_hierarchyWindow->rebuild();
		_csgObjectWindow->checkControls();
		_csgEditWindow->checkControls();
	}

	void CSGModifier::enableWindows(bool enable)
	{
		_csgObjectWindow->setVisible(enable);
		_csgEditWindow->setVisible(enable);
	}

	void CSGModifier::addCSGBrush()
	{
		switch (_brushType)
		{
		case BrushType::Cube:
			_currentCSGBrush = new CSGBrushCube();
			break;
		default:
			_currentCSGBrush = new CSGBrushCube();
			break;
		}
		
		_currentCSGBrush->setName("CSG Brush");
		_currentCSGBrush->getTransform()->setTransformMatrix(_brushMtx);
		_currentCSGBrush->getTransform()->translate(glm::vec3(0.0f, 0.5f, 0.0f));
		_currentCSGModel->addCsgBrush(_currentCSGBrush);
		_currentCSGModel->rebuild();

		_hierarchyWindow->rebuild();
		_csgObjectWindow->checkControls();
		_csgEditWindow->checkControls();
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