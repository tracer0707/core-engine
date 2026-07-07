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

	void CSGBuilder::addBrush(BrushType brushType)
	{
		CSGBrush* newBrush = nullptr;
		CSGModel* currentModel = nullptr;

		if (_models.isEmpty())
		{
			currentModel = new CSGModel(_renderer, _scene, _contentManager);
			currentModel->setName("CSG Model");
			_models.add(currentModel);
		}
		else
		{
			currentModel = _models.get(0);
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

		newBrush->setName("CSG Brush");
		newBrush->getTransform()->translate(glm::vec3(0.0f, 0.5f, 0.0f));
		currentModel->rebuild();

		auto* tree = _hierarchyWindow->getTreeView();

		TreeNode* brushNode = tree->createNode();
		brushNode->setText(newBrush->getName());
		brushNode->setObjectTag(TAG_CSG_BRUSH, newBrush);

		tree->addControl(brushNode);
		tree->selectNode(brushNode);
	}

	CSGBrush* CSGBuilder::findBrush(Core::Uuid brushId)
	{
		for (CSGModel* model : _models)
		{
			CSGBrush* brush = model->findBrush(brushId);
			if (brush != nullptr)
			{
				return brush;
			}
		}

		return nullptr;
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