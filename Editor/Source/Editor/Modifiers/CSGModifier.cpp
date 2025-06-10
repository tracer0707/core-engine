#include "CSGModifier.h"

#include <glm/gtc/matrix_transform.hpp>

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

#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"

#include "ModifierList.h"

#include "../Gizmo.h"
#include "../../Shared/Tags.h"
#include "../../CSG/CSGModel.h"
#include "../../CSG/CSGBrush.h"
#include "../../CSG/CSGBrushCube.h"

namespace Editor
{
	uint32_t CSGModifier::NAME = CSG_MODIFIER;

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

	void CSGModifier::addModel()
	{
		_currentBrush = nullptr;
		_currentModel = new CSGModel();
		_currentModel->setName("CSG Model");
		_scene->addObject(_currentModel->getObject());
		_models.add(_currentModel);

		auto* tree = _hierarchyWindow->getTreeView();

		TreeNode* modelNode = new TreeNode(_currentModel->getName(), tree);
		modelNode->setUserObject(_currentModel->getObject());
		modelNode->setUserTag(TAG_CSG_MODEL);
		tree->addControl(modelNode);
		tree->selectNode(modelNode);

		WindowManager::singleton()->invalidateAll();
	}

	void CSGModifier::addBrush(BrushType brushType)
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

		_currentBrush = newBrush;
		_currentBrush->setName("CSG Brush");
		_currentBrush->getTransform()->translate(glm::vec3(0.0f, 0.5f, 0.0f));
		_currentModel->addBrush(_currentBrush);
		_currentModel->rebuild();

		auto* tree = _hierarchyWindow->getTreeView();

		TreeNode* brushNode = new TreeNode(_currentBrush->getName(), tree);
		brushNode->setUserObject(_currentBrush->getObject());
		brushNode->setUserTag(TAG_CSG_BRUSH);

		TreeNode* modelNode = tree->findNodeByUserObject(_currentModel->getObject());
		assert(modelNode != nullptr && "TreeNode of CSG model not found");
		
		modelNode->addControl(brushNode);
		tree->selectNode(brushNode);

		WindowManager::singleton()->invalidateAll();
	}

	void CSGModifier::update()
	{
		
	}

	void CSGModifier::render()
	{
		if (_currentBrush == nullptr) return;

		glm::mat4 view = _scene->getMainCamera()->getViewMatrix();
		glm::mat4 proj = _scene->getMainCamera()->getProjectionMatrix();
		glm::mat4& model = _currentBrush->getTransform()->getTransformMatrix();

		Core::Primitives::wireCube(view, proj, model, glm::vec3(1.0f), glm::vec3(0.5f, -0.5f, 0.5f), Core::Color::RED,
			C_CCW
			| C_CULL_BACK
			| C_ENABLE_DEPTH_TEST
			| C_ENABLE_DEPTH_WRITE
			| C_ENABLE_CULL_FACE
			| C_DEPTH_LEQUAL);
	}
}