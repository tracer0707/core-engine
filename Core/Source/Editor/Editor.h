#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
	class Scene;
	class CSGModel;
	class CSGBrush;
	class Camera;
}

namespace Editor
{
	class Gizmo;

	class Editor
	{
	private:
		static Gizmo* gizmo;

		static Core::Scene* scene;
		static Core::Camera* camera;

		static Core::CSGModel* selectedCsgModel;
		static Core::CSGBrush* selectedCsgBrush;

		static int numCSGModels;
		static int numCSGBrushes;

		static glm::mat4* selectedMtx;

	public:
		static void init();
		static void free();

		static void setScene(Core::Scene* value) { scene = value; }
		static void setCamera(Core::Camera* value) { camera = value; }
		static void setSelectedMtx(glm::mat4* value) { selectedMtx = value; }

		static void renderUI();

		static void clearSelection();
		static void select(Core::CSGModel* value);
		static void select(Core::CSGBrush* value);
	};
}