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

		static bool hovered;
		static bool wasHovered;

		static bool lButtonDown;
		static bool rButtonDown;
		static bool mButtonDown;
		static bool ctrlPressed;
		static bool shiftPressed;
		static bool mouseOver;
		static glm::vec2 prevMousePos;

		static float cameraSpeed;
		static float cameraSpeedNormal;
		static float cameraSpeedFast;

		static void mouseDown(int x, int y, int mb);
		static void mouseUp(int x, int y, int mb);
		static void mouseMove(int x, int y);
		static void mouseWheel(int x, int y);

	public:
		static void init();
		static void free();

		static void setScene(Core::Scene* value) { scene = value; }
		static void setCamera(Core::Camera* value) { camera = value; }
		static void setSelectedMtx(glm::mat4* value) { selectedMtx = value; }

		static void update();
		static void render();
		static void renderUI();

		static void clearSelection();
		static void select(CSGModel* value);
		static void select(CSGBrush* value);
	};
}