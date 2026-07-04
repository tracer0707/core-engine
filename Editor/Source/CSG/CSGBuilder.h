#pragma once

#include <Core/Shared/List.h>

namespace Core
{
	class Renderer;
	class Scene;
	class ContentManager;
	class VertexBuffer;
} // namespace Core

namespace Editor
{
	class WindowManager;
	class HierarchyWindow;
	class CSGEditWindow;
	class CSGModel;
	class CSGBrush;

	class CSGBuilder
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

			Core::Renderer* _renderer = nullptr;
			Core::Scene* _scene = nullptr;
			Core::ContentManager* _contentManager = nullptr;

			WindowManager* _windowManager = nullptr;
			HierarchyWindow* _hierarchyWindow = nullptr;
			CSGEditWindow* _csgEditWindow = nullptr;

			Core::List<CSGModel*> _models;

			CSGModel* _currentModel = nullptr;
			CSGBrush* _currentBrush = nullptr;

			Core::VertexBuffer* _wireframeBuffer = nullptr;

			static CSGBuilder _singleton;

		public:
			CSGBuilder();
			~CSGBuilder();

			static CSGBuilder* singleton() { return &_singleton; }

			void setEditMode(EditMode value) { _editMode = value; }
			EditMode getEditMode() { return _editMode; }

			void addModel();
			void addBrush(BrushType brushType);

			size_t getNumModels() { return _models.count(); }
			CSGModel* getModel(int index) { return _models.get(index); }
			void removeModel(CSGModel* value) { _models.remove(value); }

			CSGModel* getCurrentModel() { return _currentModel; }
			void setCurrentModel(CSGModel* value) { _currentModel = value; }

			CSGBrush* getCurrentBrush() { return _currentBrush; }
			void setCurrentBrush(CSGBrush* value) { _currentBrush = value; }

			void init(WindowManager* windowManager, Core::Renderer* renderer, Core::Scene* scene, Core::ContentManager* contentManager);
			void destroy();
			void update();
			void render();
	};
} // namespace Editor