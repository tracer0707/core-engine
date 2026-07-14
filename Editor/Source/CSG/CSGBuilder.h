#pragma once

#include <Core/Shared/List.h>
#include <Core/Shared/Uuid.h>

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

			static CSGBuilder _singleton;

		public:
			CSGBuilder();
			~CSGBuilder();

			static CSGBuilder* singleton() { return &_singleton; }

			void setEditMode(EditMode value) { _editMode = value; }
			EditMode getEditMode() const { return _editMode; }

			const Core::List<CSGModel*>& getModels() const { return _models; }

			void addBrush(BrushType brushType);

			void init(WindowManager* windowManager, Core::Renderer* renderer, Core::Scene* scene, Core::ContentManager* contentManager);
			void destroy();
			void rebuild();
			void update();
	};
} // namespace Editor