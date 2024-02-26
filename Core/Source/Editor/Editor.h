#pragma once

namespace Core
{
	class Scene;
	class CSGModel;
	class CSGBrush;
}

namespace Editor
{
	class Editor
	{
	private:
		static Core::Scene* scene;

		static Core::CSGModel* selectedCsgModel;
		static Core::CSGBrush* selectedCsgBrush;

		static int numCSGModels;

	public:
		static void setScene(Core::Scene* value) { scene = value; }
		static void renderUI();

		static void clearSelection();
		static void select(Core::CSGModel* value);
		static void select(Core::CSGBrush* value);
	};
}