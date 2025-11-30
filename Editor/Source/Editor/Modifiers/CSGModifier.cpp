#include "CSGModifier.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Core/Content/ContentManager.h>
#include <Core/Scene/Scene.h>
#include <Core/System/InputManager.h>
#include <Core/Components/Camera.h>
#include <Core/Components/Transform.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Primitives.h>
#include <Core/Renderer/Color.h>

#include "../System/ContentLoader.h"
#include "../Windows/WindowManager.h"
#include "../Windows/HierarchyWindow.h"
#include "../Windows/CSGObjectWindow.h"
#include "../Windows/CSGEditWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"

#include "ModifierManager.h"
#include "ModifierList.h"

#include "../Gizmo.h"

#include "../../Shared/Tags.h"
#include "../../CSG/CSGModel.h"
#include "../../CSG/CSGBrush.h"
#include "../../CSG/CSGBrushCube.h"

namespace Editor
{
    uint32_t CSGModifier::NAME = CSG_MODIFIER;

    CSGModifier::CSGModifier() : Modifier(NAME) {}

    CSGModifier::~CSGModifier()
    {
        _renderer->deleteBuffer(_wireframeBuffer);
    }

    void CSGModifier::init(Core::Renderer* renderer, Core::Scene* scene, ContentLoader* contentLoader)
    {
        Modifier::init(renderer, scene, contentLoader);

        WindowManager* winMgr = ModifierManager::singleton()->getWindowManager();

        _hierarchyWindow = (HierarchyWindow*)winMgr->getWindow(HIERARCHY_WINDOW);
        _csgObjectWindow = (CSGObjectWindow*)winMgr->getWindow(CSG_OBJECT_WINDOW);
        _csgEditWindow = (CSGEditWindow*)winMgr->getWindow(CSG_EDIT_WINDOW);

        _wireframeBuffer = renderer->createBuffer(2048, 0);
    }

    void CSGModifier::enableWindows(bool enable)
    {
        _csgObjectWindow->setVisible(enable);
        _csgEditWindow->setVisible(enable);
    }

    void CSGModifier::addModel()
    {
        _currentBrush = nullptr;
        _currentModel = new CSGModel(_renderer, _scene, _contentLoader);
        _currentModel->setName("CSG Model");
        _models.add(_currentModel);

        auto* tree = _hierarchyWindow->getTreeView();

        TreeNode* modelNode = tree->createNode();
        modelNode->setText(_currentModel->getName());
        modelNode->setObjectTag(TAG_CSG_MODEL, _currentModel);
        tree->addControl(modelNode);
        tree->selectNode(modelNode);

        WindowManager* winMgr = ModifierManager::singleton()->getWindowManager();
        winMgr->invalidateAll();
    }

    void CSGModifier::addBrush(BrushType brushType)
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

        WindowManager* winMgr = ModifierManager::singleton()->getWindowManager();
        winMgr->invalidateAll();
    }

    void CSGModifier::update() {}

    void CSGModifier::render()
    {
        if (_currentBrush == nullptr) return;

        glm::mat4 view = _scene->getMainCamera()->getViewMatrix();
        glm::mat4 proj = _scene->getMainCamera()->getProjectionMatrix();
        glm::mat4 model = _currentBrush->getTransform()->getTransformMatrix();

        Core::List<int> inds = _currentBrush->getFlatIndices();
        Core::List<glm::vec3>& verts = _currentBrush->getVertices();

        Core::Primitives::wireMesh(_renderer, _wireframeBuffer, _contentLoader->getContentManager()->getDefaultMaterial(), view, proj, model, verts,
                                   inds,
                                   Core::Color::RED,
                                   Core::Primitives::WireframeMode::Polygon,
                                   C_CCW | C_CULL_BACK | C_ENABLE_DEPTH_TEST | C_ENABLE_DEPTH_WRITE | C_ENABLE_CULL_FACE | C_DEPTH_LEQUAL);
    }
} // namespace Editor