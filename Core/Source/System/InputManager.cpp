#include "InputManager.h"

#include <algorithm>
#include <iostream>
#include <imgui.h>

#include "../Shared/Guid.h"

#include <sdl/SDL_events.h>

namespace Core
{
	InputManager InputManager::singleton;

	InputManager::InputManager()
	{
		for (int i = 0; i < 1024; ++i)
			keyStates[i] = false;

		reset();
	}

	InputManager::~InputManager()
	{
		
	}

	std::string InputManager::subscribeMouseMoveEvent(MouseEvent callback)
	{
		std::string guid = Guid::genGuid();
		mouseMoveEvents.push_back(std::make_pair(guid, callback));

		return guid;
	}

	std::string InputManager::subscribeMouseDownEvent(MouseButtonEvent callback)
	{
		std::string guid = Guid::genGuid();
		mouseDownEvents.push_back(std::make_pair(guid, callback));

		return guid;
	}

	std::string InputManager::subscribeMouseUpEvent(MouseButtonEvent callback)
	{
		std::string guid = Guid::genGuid();
		mouseUpEvents.push_back(std::make_pair(guid, callback));
		
		return guid;
	}

	std::string InputManager::subscribeMouseWheelEvent(MouseEvent callback)
	{
		std::string guid = Guid::genGuid();
		mouseWheelEvents.push_back(std::make_pair(guid, callback));
		
		return guid;
	}

	void InputManager::unsubscribeMouseMoveEvent(std::string id)
	{
		auto it = std::find_if(mouseMoveEvents.begin(), mouseMoveEvents.end(), [=](std::pair<std::string, MouseEvent>& evt) -> bool
			{
				return evt.first == id;
			}
		);

		if (it != mouseMoveEvents.end())
			mouseMoveEvents.erase(it);
	}

	void InputManager::unsubscribeMouseDownEvent(std::string id)
	{
		auto it = std::find_if(mouseDownEvents.begin(), mouseDownEvents.end(), [=](std::pair<std::string, MouseButtonEvent>& evt) -> bool
			{
				return evt.first == id;
			}
		);

		if (it != mouseDownEvents.end())
			mouseDownEvents.erase(it);
	}

	void InputManager::unsubscribeMouseUpEvent(std::string id)
	{
		auto it = std::find_if(mouseUpEvents.begin(), mouseUpEvents.end(), [=](std::pair<std::string, MouseButtonEvent>& evt) -> bool
			{
				return evt.first == id;
			}
		);

		if (it != mouseUpEvents.end())
			mouseUpEvents.erase(it);
	}

	void InputManager::unsubscribeMouseWheelEvent(std::string id)
	{
		auto it = std::find_if(mouseWheelEvents.begin(), mouseWheelEvents.end(), [=](std::pair<std::string, MouseEvent>& evt) -> bool
			{
				return evt.first == id;
			}
		);

		if (it != mouseWheelEvents.end())
			mouseWheelEvents.erase(it);
	}

	std::string InputManager::subscribeKeyDownEvent(KeyboardEvent callback)
	{
		std::string guid = Guid::genGuid();
		keyDownEvents.push_back(std::make_pair(guid, callback));
		
		return guid;
	}

	std::string InputManager::subscribeKeyUpEvent(KeyboardEvent callback)
	{
		std::string guid = Guid::genGuid();
		keyUpEvents.push_back(std::make_pair(guid, callback));
		
		return guid;
	}

	void InputManager::unsubscribeKeyDownEvent(std::string id)
	{
		auto it = std::find_if(keyDownEvents.begin(), keyDownEvents.end(), [=](std::pair<std::string, KeyboardEvent>& evt) -> bool
			{
				return evt.first == id;
			}
		);

		if (it != keyDownEvents.end())
			keyDownEvents.erase(it);
	}

	void InputManager::unsubscribeKeyUpEvent(std::string id)
	{
		auto it = std::find_if(keyUpEvents.begin(), keyUpEvents.end(), [=](std::pair<std::string, KeyboardEvent>& evt) -> bool
			{
				return evt.first == id;
			}
		);

		if (it != keyUpEvents.end())
			keyUpEvents.erase(it);
	}

	void InputManager::mouseMoveEvent(int x, int y)
	{
		for (auto& it : mouseMoveEvents)
		{
			if (it.second != nullptr)
				it.second(x, y);
		}
	}

	void InputManager::mouseDownEvent(MouseButton mb, int x, int y)
	{
		for (auto& it : mouseDownEvents)
		{
			if (it.second != nullptr)
				it.second(mb, x, y);
		}
	}

	void InputManager::mouseUpEvent(MouseButton mb, int x, int y)
	{
		for (auto& it : mouseUpEvents)
		{
			if (it.second != nullptr)
				it.second(mb, x, y);
		}
	}

	void InputManager::mouseWheelEvent(int x, int y)
	{
		for (auto& it : mouseWheelEvents)
		{
			if (it.second != nullptr)
				it.second(x, y);
		}
	}

	void InputManager::updateKeys(void* event)
	{
		SDL_Event* evt = (SDL_Event*)event;

		//Handle input events
		if (evt->type == SDL_MOUSEMOTION)
		{
			mouseMoveEvent(evt->motion.x, evt->motion.y);
		}

		if (evt->type == SDL_MOUSEBUTTONDOWN)
		{
			if (evt->button.button == SDL_BUTTON_LEFT)
			{
				mouseDownEvent(InputManager::MouseButton::MBE_LEFT, evt->button.x, evt->button.y);
				setMouseButton(0, true);
				setMouseButtonDown(0, true);

				int button = 0;
				void* args[1] = { &button };
			}

			if (evt->button.button == SDL_BUTTON_RIGHT)
			{
				mouseDownEvent(InputManager::MouseButton::MBE_RIGHT, evt->button.x, evt->button.y);
				setMouseButton(1, true);
				setMouseButtonDown(1, true);

				int button = 1;
				void* args[1] = { &button };
			}

			if (evt->button.button == SDL_BUTTON_MIDDLE)
			{
				mouseDownEvent(InputManager::MouseButton::MBE_MIDDLE, evt->button.x, evt->button.y);
				setMouseButton(2, true);
				setMouseButtonDown(2, true);

				int button = 2;
				void* args[1] = { &button };
			}
		}

		if (evt->type == SDL_MOUSEBUTTONUP)
		{
			if (evt->button.button == SDL_BUTTON_LEFT)
			{
				mouseUpEvent(InputManager::MouseButton::MBE_LEFT, evt->button.x, evt->button.y);
				setMouseButton(0, false);
				setMouseButtonUp(0, true);

				int button = 0;
				void* args[1] = { &button };
			}

			if (evt->button.button == SDL_BUTTON_RIGHT)
			{
				mouseUpEvent(InputManager::MouseButton::MBE_RIGHT, evt->button.x, evt->button.y);
				setMouseButton(1, false);
				setMouseButtonUp(1, true);

				int button = 1;
				void* args[1] = { &button };
			}

			if (evt->button.button == SDL_BUTTON_MIDDLE)
			{
				mouseUpEvent(InputManager::MouseButton::MBE_MIDDLE, evt->button.x, evt->button.y);
				setMouseButton(2, false);
				setMouseButtonUp(2, true);

				int button = 2;
				void* args[1] = { &button };
			}
		}

		if (evt->type == SDL_MOUSEWHEEL)
		{
			mouseWheelEvent(evt->wheel.x, evt->wheel.y);
			setMouseWheel(evt->wheel.x, evt->wheel.y);
		}

		if (evt->type == SDL_KEYDOWN)
		{
			setKey(evt->key.keysym.scancode, true);

			if (evt->key.repeat == 0)
			{
				setKeyDown(evt->key.keysym.scancode, true);

				int key = (int)evt->key.keysym.scancode;

				for (auto& kevent : keyDownEvents)
				{
					if (kevent.second != nullptr)
						kevent.second(key);
				}
			}
		}

		if (evt->type == SDL_KEYUP)
		{
			setKey(evt->key.keysym.scancode, false);

			if (evt->key.repeat == 0)
			{
				setKeyUp(evt->key.keysym.scancode, true);

				int key = (int)evt->key.keysym.scancode;

				for (auto& kevent : keyUpEvents)
				{
					if (kevent.second != nullptr)
						kevent.second(key);
				}
			}
		}
	}

	void InputManager::updateMouse(void* window)
	{
		int _gx = 0;
		int _gy = 0;

		int _x = 0;
		int _y = 0;

		SDL_GetGlobalMouseState(&_gx, &_gy);
		setMousePosition(_gx, _gy);

		SDL_GetMouseState(&_x, &_y);
		setMouseRelativePosition(_x, _y);

		if (!cursorVisible)
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);

		if (cursorLocked)
		{
			SDL_Window* _window = (SDL_Window*)window;
			int w = 0;
			int h = 0;
			SDL_GetWindowSize(_window, &w, &h);

			if (prevCursorLocked == cursorLocked)
				setMouseDirection(_x - (w / 2), _y - (h / 2));

			SDL_WarpMouseInWindow(_window, w / 2, h / 2);

			if (prevCursorLocked != cursorLocked)
			{
				++cursorFrame;
				if (cursorFrame > 1)
				{
					prevCursorLocked = cursorLocked;
					cursorFrame = 0;
				}
			}

			prevMouseX = _gx;
			prevMouseY = _gy;
		}
		else
		{
			int rx = _gx - prevMouseX;
			int ry = _gy - prevMouseY;

			setMouseDirection(rx, ry);

			prevMouseX = _gx;
			prevMouseY = _gy;

			prevCursorLocked = cursorLocked;
		}
	}

	bool InputManager::getMouseButton(int button)
	{
		return mouseStates[button];
	}

	void InputManager::setMouseButton(int button, bool pressed)
	{
		mouseStates[button] = pressed;
	}

	bool InputManager::getMouseButtonDown(int button)
	{
		return mouseDownStates[button];
	}

	void InputManager::setMouseButtonDown(int button, bool state)
	{
		mouseDownStates[button] = state;
	}

	bool InputManager::getMouseButtonUp(int button)
	{
		return mouseUpStates[button];
	}

	void InputManager::setMouseButtonUp(int button, bool state)
	{
		mouseUpStates[button] = state;
	}

	std::pair<int, int> InputManager::getMouseWheel()
	{
		return mouseWheel;
	}

	void InputManager::setMouseWheel(int x, int y)
	{
		mouseWheel = std::make_pair(x, y);
	}

	std::pair<int, int> InputManager::getMouseDirection()
	{
		return mouseDir;
	}

	void InputManager::setMouseDirection(int x, int y)
	{
		mouseDir = std::make_pair(x, y);
	}

	std::pair<int, int> InputManager::getMousePosition()
	{
		return mousePos;
	}

	void InputManager::setMousePosition(int x, int y)
	{
		mousePos = std::make_pair(x, y);
	}

	std::pair<int, int> InputManager::getMouseRelativePosition()
	{
		return relativeMousePos;
	}

	void InputManager::setCursorLocked(bool value)
	{
		if (cursorLocked != value)
		{
			prevCursorLocked = cursorLocked;
			cursorLocked = value;
			cursorFrame = 0;
		}
	}

	void InputManager::setCursorVisible(bool value)
	{
		cursorVisible = value;
		SDL_ShowCursor(value);
	}

	void InputManager::setMouseRelativePosition(int x, int y)
	{
		relativeMousePos = std::make_pair(x, y);
	}

	bool InputManager::getKey(int key)
	{
		return keyStates[key];
	}

	void InputManager::setKey(int key, bool pressed)
	{
		keyStates[key] = pressed;
	}

	bool InputManager::getKeyDown(int key)
	{
		return keyDownStates[key];
	}

	void InputManager::setKeyDown(int key, bool state)
	{
		keyDownStates[key] = state;
	}

	bool InputManager::getKeyUp(int key)
	{
		return keyUpStates[key];
	}

	void InputManager::setKeyUp(int key, bool state)
	{
		keyUpStates[key] = state;
	}

	void InputManager::reset()
	{
		for (std::map<int, bool>::iterator it = keyDownStates.begin(); it != keyDownStates.end(); ++it)
			it->second = false;

		for (std::map<int, bool>::iterator it = keyUpStates.begin(); it != keyUpStates.end(); ++it)
			it->second = false;

		for (int i = 0; i < 32; ++i)
		{
			mouseDownStates[i] = false;
			mouseUpStates[i] = false;
		}

		setMouseDirection(0, 0);
		setMouseWheel(0, 0);
	}
}