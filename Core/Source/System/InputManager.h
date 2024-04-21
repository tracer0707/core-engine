#pragma once

#include <functional>
#include <map>
#include <vector>
#include <string>

namespace GX
{
	class InputManager
	{
	public:
		enum MouseButton { MBE_LEFT, MBE_RIGHT, MBE_MIDDLE };

		typedef std::function<void(int x, int y)> MouseEvent;
		typedef std::function<void(MouseButton mb, int x, int y)> MouseButtonEvent;
		typedef std::function<void(int key)> KeyboardEvent;

	private:
		static InputManager singleton;

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

		std::vector<std::pair<std::string, MouseEvent>> mouseMoveEvents;
		std::vector<std::pair<std::string, MouseButtonEvent>> mouseDownEvents;
		std::vector<std::pair<std::string, MouseButtonEvent>> mouseUpEvents;
		std::vector<std::pair<std::string, MouseEvent>> mouseWheelEvents;

		std::vector<std::pair<std::string, KeyboardEvent>> keyDownEvents;
		std::vector<std::pair<std::string, KeyboardEvent>> keyUpEvents;

		void mouseMoveEvent(int x, int y);
		void mouseDownEvent(MouseButton mb, int x, int y);
		void mouseUpEvent(MouseButton mb, int x, int y);
		void mouseWheelEvent(int x, int y);

		void setMouseButton(int button, bool pressed);
		void setMouseButtonDown(int button, bool state);
		void setMouseButtonUp(int button, bool state);
		void setMouseWheel(int x, int y);
		void SetKey(int key, bool pressed);
		void setKeyDown(int key, bool state);
		void SetKeyUp(int key, bool state);
		void setMouseDirection(int x, int y);
		void SetMousePosition(int x, int y);
		void setMouseRelativePosition(int x, int y);

	public:
		enum EventType { MouseMove, MouseDown, MouseUp, MouseScrollWheel, KeyDown, KeyUp, KeyPress };

		InputManager();
		~InputManager();

		static InputManager* getSingleton() { return &singleton; }

		std::string subscribeMouseMoveEvent(MouseEvent callback);
		std::string subscribeMouseDownEvent(MouseButtonEvent callback);
		std::string subscribeMouseUpEvent(MouseButtonEvent callback);
		std::string subscribeMouseWheelEvent(MouseEvent callback);

		void unsubscribeMouseMoveEvent(std::string id);
		void unsubscribeMouseDownEvent(std::string id);
		void unsubscribeMouseUpEvent(std::string id);
		void unsubscribeMouseWheelEvent(std::string id);

		std::string subscribeKeyDownEvent(KeyboardEvent callback);
		std::string subscribeKeyUpEvent(KeyboardEvent callback);
		void unsubscribeKeyDownEvent(std::string id);
		void unsubscribeKeyUpEvent(std::string id);

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