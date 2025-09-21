#pragma once

#include <functional>

#include "../Shared/String.h"

namespace Core
{
	class Renderer;
	class AssetManager;
	class Time;
	class InputManager;
	class EventHandler;
	class Application;

	class Window
	{
		friend class Application;

	private:
		bool _opened = false;

		std::function<void()> _onClose = nullptr;

		void processEvents(void* event);
		void internalUpdate();
		
		virtual void update() {}
		virtual void render() {}

	protected:
		Window(Application* application, String title, int width, int height);
		virtual ~Window();

		void* _ctx = nullptr;
		Application* _application = nullptr;
		Renderer* _renderer = nullptr;
		AssetManager* _assetManager = nullptr;
		Time* _time = nullptr;
		InputManager* _inputManager = nullptr;
		EventHandler* _eventHandler = nullptr;

		int _width = 0;
		int _height = 0;

	public:
		void* getContext() { return _ctx; }
		Renderer* getRenderer() { return _renderer; }
		AssetManager* getAssetManager() { return _assetManager; }

		void setTitle(String title);
		
		int getWidth() { return _width; }
		int getHeight() { return _height; }

		void setOnClose(std::function<void()> event) { _onClose = event; }

		void close();
	};
}