#pragma once

#include <Core/Shared/List.h>

#include "Modifier.h"

namespace Core
{
    class VertexBuffer;
}

namespace Editor
{
    class HierarchyWindow;
    class CSGObjectWindow;
    class CSGEditWindow;
    class CSGModel;
    class CSGBrush;

    class CSGModifier : public Modifier
    {
      public:
        enum class EditMode
        {
            Select,
            EditVertices,
            EditEdges,
            EditFaces
        };

        enum class BrushType
        {
            Cube,
            Sphere,
            Cylinder,
            Cone,
            Stair,
            Polygon
        };

      private:
        EditMode _editMode = EditMode::Select;

        HierarchyWindow* _hierarchyWindow = nullptr;
        CSGObjectWindow* _csgObjectWindow = nullptr;
        CSGEditWindow* _csgEditWindow = nullptr;

        Core::List<CSGModel*> _models;

        CSGModel* _currentModel = nullptr;
        CSGBrush* _currentBrush = nullptr;

        Core::VertexBuffer* _wireframeBuffer = nullptr;

        virtual void enableWindows(bool enable);

      public:
        CSGModifier();
        virtual ~CSGModifier();

        static uint32_t NAME;

        void setEditMode(EditMode value) { _editMode = value; }
        EditMode getEditMode() { return _editMode; }

        void addModel();
        void addBrush(BrushType brushType);

        int getNumModels() { return _models.count(); }
        CSGModel* getModel(int index) { return _models.get(index); }
        void removeModel(CSGModel* value) { _models.remove(value); }

        CSGModel* getCurrentModel() { return _currentModel; }
        void setCurrentModel(CSGModel* value) { _currentModel = value; }

        CSGBrush* getCurrentBrush() { return _currentBrush; }
        void setCurrentBrush(CSGBrush* value) { _currentBrush = value; }

        virtual void init(Core::Renderer* renderer, Core::Scene* scene, Core::AssetManager* assetManager);
        virtual void update();
        virtual void render();
    };
} // namespace Editor