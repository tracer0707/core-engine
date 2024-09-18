#pragma once

#include "Modifier.h"

namespace Core
{
	class Camera;
}

namespace Editor
{
	class CSGModifier : public Modifier
	{
	public:
		enum class EditMode
		{
			AddBrush,
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
		EditMode _editMode = EditMode::AddBrush;
		BrushType _brushType = BrushType::Cube;

	public:
		CSGModifier();
		virtual ~CSGModifier();

		static UString NAME;

		void setEditMode(EditMode value) { _editMode = value; }
		EditMode getEditMode() { return _editMode; }

		void setBrushType(BrushType value) { _brushType = value; }
		BrushType getBrushType() { return _brushType; }

		virtual void init(Core::Scene* scene);
		virtual void update();
		virtual void render();
	};
}