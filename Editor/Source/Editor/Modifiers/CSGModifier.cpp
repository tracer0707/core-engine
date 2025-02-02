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
	}

	void CSGModifier::enableWindows(bool enable)
	{
		_csgObjectWindow->setVisible(enable);
		_csgEditWindow->setVisible(enable);
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

	void CSGModifier::addCSGBrush(BrushType brushType)
	{
		CSGBrush* newBrush = nullptr;

		switch (brushType)
		{
		case BrushType::Cube:
			newBrush = new CSGBrushCube();
			break;
		default:
			break;
		}

		if (newBrush == nullptr) return;

		_brushMtx = glm::identity<glm::mat4>();
		
		_currentCSGBrush = newBrush;
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
		
	}
}