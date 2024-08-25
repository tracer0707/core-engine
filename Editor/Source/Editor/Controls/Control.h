#pragma once

#include "Container.h"

namespace Editor
{
	class Window;
	class Container;

	class Control: public Container
	{
		friend class Container;

	private:
		Container* _parent = nullptr;

	public:
		Control();
		virtual ~Control();

		virtual void update() = 0;
	};
}