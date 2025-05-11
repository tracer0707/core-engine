#pragma once

#include <functional>
#include <map>
#include <vector>
#include <string>

#include "../Shared/Uuid.h"

namespace Core
{
	class InputManager
	{
	public:
		enum MouseButton { MBE_LEFT, MBE_RIGHT, MBE_MIDDLE };

		typedef std::function<void(int x, int y)> MouseEvent;
		typedef std::function<void(MouseButton mb, int x, int y)> MouseButtonEvent;
		typedef std::function<void(int key)> KeyboardEvent;

	private:
		static InputManager _singleton;

		std::pair<int, int> mouseDir;
		std::pair<int, int> mouseWheel;
		std::pair<int, int> mousePos;
		std::pair<int, int> relativeMousePos;

		std::map<int, bool> keyStates;
		std::map<int, bool> keyDownStates;
		std::map<int, bool> keyUpStates;

		bool mouseStates[32];
		bool mouseDownStates[32];
		bool mouseUpStates[32];

		int cursorFrame = 0;
		bool prevCursorLocked = false;
		bool cursorLocked = false;
		bool cursorVisible = true;

		int prevMouseX = 0;
		int prevMouseY = 0;

		std::vector<std::pair<Uuid, MouseEvent>> mouseMoveEvents;
		std::vector<std::pair<Uuid, MouseButtonEvent>> mouseDownEvents;
		std::vector<std::pair<Uuid, MouseButtonEvent>> mouseUpEvents;
		std::vector<std::pair<Uuid, MouseEvent>> mouseWheelEvents;

		std::vector<std::pair<Uuid, KeyboardEvent>> keyDownEvents;
		std::vector<std::pair<Uuid, KeyboardEvent>> keyUpEvents;

		void mouseMoveEvent(int x, int y);
		void mouseDownEvent(MouseButton mb, int x, int y);
		void mouseUpEvent(MouseButton mb, int x, int y);
		void mouseWheelEvent(int x, int y);

		void setMouseButton(int button, bool pressed);
		void setMouseButtonDown(int button, bool state);
		void setMouseButtonUp(int button, bool state);
		void setMouseWheel(int x, int y);
		void setKey(int key, bool pressed);
		void setKeyDown(int key, bool state);
		void setKeyUp(int key, bool state);
		void setMouseDirection(int x, int y);
		void setMousePosition(int x, int y);
		void setMouseRelativePosition(int x, int y);

	public:
		enum EventType { MouseMove, MouseDown, MouseUp, MouseScrollWheel, KeyDown, KeyUp, KeyPress };

		InputManager();
		~InputManager();

		static InputManager* singleton() { return &_singleton; }

		Uuid subscribeMouseMoveEvent(MouseEvent callback);
		Uuid subscribeMouseDownEvent(MouseButtonEvent callback);
		Uuid subscribeMouseUpEvent(MouseButtonEvent callback);
		Uuid subscribeMouseWheelEvent(MouseEvent callback);

		void unsubscribeMouseMoveEvent(Uuid id);
		void unsubscribeMouseDownEvent(Uuid id);
		void unsubscribeMouseUpEvent(Uuid id);
		void unsubscribeMouseWheelEvent(Uuid id);

		Uuid subscribeKeyDownEvent(KeyboardEvent callback);
		Uuid subscribeKeyUpEvent(KeyboardEvent callback);
		void unsubscribeKeyDownEvent(Uuid id);
		void unsubscribeKeyUpEvent(Uuid id);

		void updateKeys(void* event); //SDL_event
		void updateMouse(void* window);
		bool getMouseButton(int button);
		bool getMouseButtonDown(int button);
		bool getMouseButtonUp(int button);
		std::pair<int, int> getMouseWheel();
		
		bool getKey(int key);
		bool getKeyDown(int key);
		bool getKeyUp(int key);

		void reset();
		std::pair<int, int> getMouseDirection();
		std::pair<int, int> getMousePosition();
		std::pair<int, int> getMouseRelativePosition();

		void setCursorLocked(bool value);
		bool getCursorLocked() { return cursorLocked; }

		void setCursorVisible(bool value);
		bool getCursorVisible() { return cursorVisible; }
	};
}