#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <Shared/List.h>

#include "Modifier.h"

namespace Core
{
	class Camera;
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

		Core::List<CSGModel*> _csgModels;

		CSGModel* _currentCSGModel = nullptr;
		CSGBrush* _currentCSGBrush = nullptr;

		glm::mat4 _brushMtx = glm::identity<glm::mat4>();

		virtual void enableWindows(bool enable);

	public:
		CSGModifier();
		virtual ~CSGModifier();

		static UString NAME;

		void setEditMode(EditMode value) { _editMode = value; }
		EditMode getEditMode() { return _editMode; }

		void addCSGModel();
		void addCSGBrush(BrushType brushType);

		int getNumCsgModels() { return _csgModels.count(); }
		CSGModel* getCsgModel(int index) { return _csgModels.get(index); }
		void removeCsgModel(CSGModel* value) { _csgModels.remove(value); }

		CSGModel* getCurrentCsgModel() { return _currentCSGModel; }
		CSGBrush* getCurrentCsgBrush() { return _currentCSGBrush; }

		virtual void init(Core::Scene* scene);
		virtual void update();
		virtual void render();
	};
}