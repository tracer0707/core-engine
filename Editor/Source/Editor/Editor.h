#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <Shared/List.h>

namespace Core
{
	class Scene;
	class Camera;
}

namespace Editor
{
	class Gizmo;
	class CSGModel;
	class CSGBrush;

	class Editor
	{
	private:
		static Gizmo* gizmo;

		static Core::Scene* scene;
		static Core::Camera* camera;

		static CSGModel* selectedCsgModel;
		static CSGBrush* selectedCsgBrush;

		static int numCSGModels;
		static int numCSGBrushes;

		static glm::mat4* selectedMtx;

		static Core::List<CSGModel*> csgModels;

	public:
		static void init();
		static void free();

		static void setScene(Core::Scene* value) { scene = value; }
		static void setCamera(Core::Camera* value) { camera = value; }
		static void setSelectedMtx(glm::mat4* value) { selectedMtx = value; }

		static void renderUI();

		static void clearSelection();
		static void select(CSGModel* value);
		static void select(CSGBrush* value);
	};
}