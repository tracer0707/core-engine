#pragma once

#include <string>

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
		std::string _id = "";
		Container* _parent = nullptr;
		Style _style;
		bool _visible = true;

	public:
		Control();
		virtual ~Control();

		Style& getStyle() { return _style; }

		void setVisible(bool value) { _visible = value; }
		bool getVisible() { return _visible; }

		void setEnabled(bool value) { _style.enabled = value; }
		bool getEnabled() { return _style.enabled; }

		virtual void update() = 0;
	};
}