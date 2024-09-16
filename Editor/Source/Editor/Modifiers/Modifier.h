#pragma once

#include <Shared/String.h>
#include <Shared/List.h>

namespace Core
{
	class Scene;
}

namespace Editor
{
	class Window;

	class Modifier
	{
	private:
		UString _name = "";

		Core::List<Window*> _windows;
		Core::Scene* _scene = nullptr;

	public:
		Modifier(UString name);
		virtual ~Modifier();

		UString getName() { return _name; }

		void addWindow(Window* value) { _windows.add(value); }
		int getWindowCount() { return _windows.count(); }
		Window* getWindow(int index) { return _windows.get(index); }

		virtual void init(Core::Scene* scene) = 0 { _scene = scene; }
		virtual void update() = 0;
		virtual void render() = 0;
	};
}