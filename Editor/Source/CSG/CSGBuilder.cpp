#include "CSGBuilder.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Core/Content/ContentManager.h>
#include <Core/Scene/Scene.h>
#include <Core/Scene/Object.h>
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
	}

	void CSGBuilder::destroy()
	{
	}

	void CSGBuilder::addBrush(BrushType brushType)
	{
		CSGBrush* newBrush = nullptr;
		CSGModel* currentModel = nullptr;
		TreeNode* modelNode = nullptr;

		auto* tree = _hierarchyWindow->getTreeView();

		if (tree->getSelectedNodes().count() > 0)
		{
			for (TreeNode* node : tree->getSelectedNodes())
			{
				if (node->getObjectTag(TAG_CSG_MODEL) != nullptr)
				{
					currentModel = (CSGModel*)node->getObjectTag(TAG_CSG_MODEL);
					modelNode = node;
					break;
				}
				else if (node->getObjectTag(TAG_CSG_BRUSH) != nullptr)
				{
					CSGBrush* selectedBrush = (CSGBrush*)node->getObjectTag(TAG_CSG_BRUSH);
					currentModel = selectedBrush->getParent();
					break;
				}
			}
		}
		else
		{
			currentModel = new CSGModel(_renderer, _scene, _contentManager);
			currentModel->getObject()->setName("CSG Model");
			_models.add(currentModel);

			modelNode = tree->createNode();
			modelNode->setText(currentModel->getObject()->getName());
			modelNode->setObjectTag(TAG_CSG_MODEL, currentModel);
			tree->addControl(modelNode);
		}

		switch (brushType)
		{
		case BrushType::Cube:
			newBrush = currentModel->createCubeBrush();
			break;
		default:
			break;
		}

		if (newBrush == nullptr) return;

		newBrush->getObject()->setName("CSG Brush");
		newBrush->getObject()->getTransform()->translate(glm::vec3(0.0f, 0.5f, 0.0f));
		currentModel->rebuild();

		TreeNode* brushNode = tree->createNode();
		brushNode->setText(newBrush->getObject()->getName());
		brushNode->setObjectTag(TAG_CSG_BRUSH, newBrush);

		tree->addControl(brushNode);
		tree->selectNode(brushNode);
	}

	void CSGBuilder::rebuild()
	{
		for (CSGModel* model : _models)
		{
			model->rebuild();
		}
	}

	void CSGBuilder::update() {}
} // namespace Editor