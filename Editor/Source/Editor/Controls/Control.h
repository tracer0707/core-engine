#pragma once

#include "Container.h"
#include "Style.h"

namespace Editor
{
	class Window;
	class Container;

	class Control: public Container
	{
		friend class Container;

	protected:
		Container* _parent = nullptr;

		Style _style;

		bool _visible = true;

	public:
		Control();
		virtual ~Control();

		Style& getStyle() { return _style; }

		void setVisible(bool value) { _visible = value; }
		bool getVisible() { return _visible; }

		virtual void update() = 0;
	};
}