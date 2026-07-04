#include "CSGBuilder.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Core/Content/ContentManager.h>
#include <Core/Scene/Scene.h>
#include <Core/System/InputManager.h>
#include <Core/Components/Camera.h>
#include <Core/Components/Transform.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Primitives.h>
#include <Core/Renderer/Color.h>

#include "CSGModel.h"
#include "CSGBrush.h"
#include "CSGBrushCube.h"

#include "../Editor/Windows/WindowManager.h"
#include "../Editor/Windows/HierarchyWindow.h"
#include "../Editor/Windows/CSGEditWindow.h"

#include "../Editor/Controls/TreeView.h"
#include "../Editor/Controls/TreeNode.h"

#include "../Editor/Gizmo.h"
#include "../Shared/Tags.h"

namespace Editor
{
	CSGBuilder CSGBuilder::_singleton;

	CSGBuilder::CSGBuilder() {}

	CSGBuilder::~CSGBuilder() {}

	void CSGBuilder::init(WindowManager* windowManager, Core::Renderer* renderer, Core::Scene* scene, Core::ContentManager* contentManager)
	{
		_windowManager = windowManager;
		_renderer = renderer;
		_scene = scene;
		_contentManager = contentManager;

		_hierarchyWindow = (HierarchyWindow*)_windowManager->getWindow(HIERARCHY_WINDOW);
		_csgEditWindow = (CSGEditWindow*)_windowManager->getWindow(CSG_EDIT_WINDOW);

		_wireframeBuffer = renderer->createBuffer(nullptr, 2048, nullptr, 0);
	}

	void CSGBuilder::destroy()
	{
		_renderer->deleteBuffer(_wireframeBuffer);
	}

	void CSGBuilder::addModel()
	{
		_currentBrush = nullptr;
		_currentModel = new CSGModel(_renderer, _scene, _contentManager);
		_currentModel->setName("CSG Model");
		_models.add(_currentModel);

		auto* tree = _hierarchyWindow->getTreeView();

		TreeNode* modelNode = tree->createNode();
		modelNode->setText(_currentModel->getName());
		modelNode->setObjectTag(TAG_CSG_MODEL, _currentModel);
		tree->addControl(modelNode);
		tree->selectNode(modelNode);
	}

	void CSGBuilder::addBrush(BrushType brushType)
	{
		CSGBrush* newBrush = nullptr;

		switch (brushType)
		{
		case BrushType::Cube:
			newBrush = _currentModel->createCubeBrush();
			break;
		default:
			break;
		}

		if (newBrush == nullptr) return;

		_currentBrush = newBrush;
		_currentBrush->setName("CSG Brush");
		_currentBrush->getTransform()->translate(glm::vec3(0.0f, 0.5f, 0.0f));
		_currentModel->rebuild();

		auto* tree = _hierarchyWindow->getTreeView();

		TreeNode* brushNode = tree->createNode();
		brushNode->setText(_currentBrush->getName());
		brushNode->setObjectTag(TAG_CSG_BRUSH, _currentBrush);

		TreeNode* modelNode = tree->findNodeByTag(TAG_CSG_MODEL, _currentModel);
		assert(modelNode != nullptr && "TreeNode of CSG model not found");

		modelNode->addControl(brushNode);
		tree->selectNode(brushNode);
	}

	void CSGBuilder::update() {}

	void CSGBuilder::render()
	{
		if (_currentBrush == nullptr) return;

		glm::mat4 view = _scene->getMainCamera()->getViewMatrix();
		glm::mat4 proj = _scene->getMainCamera()->getProjectionMatrix();
		glm::mat4 model = _currentBrush->getTransform()->getTransformMatrix();

		Core::List<int> inds = _currentBrush->getFlatIndices();
		Core::List<glm::vec3>& verts = _currentBrush->getVertices();

		Core::Primitives::wireMesh(_renderer, _wireframeBuffer, view, proj, model, verts, inds, Core::Color::RED,
								   Core::Primitives::WireframeMode::Polygon,
								   C_CCW | C_CULL_BACK | C_ENABLE_DEPTH_TEST | C_ENABLE_DEPTH_WRITE | C_ENABLE_CULL_FACE | C_DEPTH_LEQUAL);
	}
} // namespace Editor