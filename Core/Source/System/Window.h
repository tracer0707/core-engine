#pragma once

#include <functional>

#include "../Shared/String.h"

namespace Core
{
	class Renderer;
	class AssetManager;
	class Time;
	class InputManager;

	class Window
	{
		friend class Application;

	private:
		void internalUpdate(bool& isRunning);
		
		virtual void update() {}
		virtual void render() {}

	protected:
		Window(UString title, int width, int height);
		virtual ~Window();

		void* _ctx = nullptr;
		Renderer* _renderer = nullptr;
		AssetManager* _assetManager = nullptr;
		Time* _time = nullptr;
		InputManager* _inputManager = nullptr;

		int _width = 0;
		int _height = 0;

	public:
		void* getContext() { return _ctx; }
		Renderer* getRenderer() { return _renderer; }
		AssetManager* getAssetManager() { return _assetManager; }

		void setTitle(UString title);
		
		int getWidth() { return _width; }
		int getHeight() { return _height; }
	};
}