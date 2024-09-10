#pragma once

#include <Shared/String.h>
#include <Shared/List.h>

namespace Editor
{
	class Window;

	class Modifier
	{
	private:
		UString _name = "";

		Core::List<Window*> _windows;

	public:
		Modifier(UString name);
		virtual ~Modifier();

		UString getName() { return _name; }

		void addWindow(Window* value) { _windows.add(value); }
		int getWindowCount() { return _windows.count(); }
		Window* getWindow(int index) { return _windows.get(index); }
	};
}