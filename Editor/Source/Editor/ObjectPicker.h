#pragma once

#include <Core/Shared/Uuid.h>

namespace Core
{
	class Scene;
	class Camera;
	class Mesh;
	class Object;
	class VertexBuffer;
	class Renderer;
}

namespace Editor
{
	class WindowManager;
	class CSGBrush;

	class ObjectPicker
	{
	private:
		WindowManager* _windowManager = nullptr;

		Core::Scene* _scene = nullptr;
		Core::Camera* _camera = nullptr;
		Core::Object* _selectedObject = nullptr;
		Core::Mesh* _selectedMesh = nullptr;
		Core::VertexBuffer* _highlightBuffer = nullptr;
		Core::Renderer* _renderer = nullptr;

		Core::Uuid _mouseDownEventId = Core::Uuid::Empty;
		Core::Uuid _mouseMoveEventId = Core::Uuid::Empty;
		Core::Uuid _mouseUpEventId = Core::Uuid::Empty;

		CSGBrush* _selectedCsgBrush = nullptr;
		size_t _selectedCsgFace = -1;

		bool _isGizmoWasUsed = false;
		bool _isMouseInView = false;
		bool _isMouseWasMoved = false;

		bool _isMouseLPressed = false;
		bool _isMouseRPressed = false;
		bool _isMouseMPressed = false;

		float _offsetX = 0;
		float _offsetY = 0;

		void pickObject(int x, int y);
		void highlightCsgBrush(int x, int y);

		static ObjectPicker _singleton;

	public:
		static ObjectPicker* singleton() { return &_singleton; }

		void init(WindowManager* windowManager, Core::Renderer* renderer, Core::Scene* scene, Core::Camera* camera);
		void destroy();

		void update(bool isMouseInView, bool isGizmoWasUsed, float offsetX, float offsetY);
		void render();
	};
}