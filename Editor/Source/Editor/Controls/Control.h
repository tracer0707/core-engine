#pragma once

#include <string>
#include <map>

#include "Container.h"
#include "Style.h"

#include <Core/Shared/String.h>

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
		std::map<int, void*> _objectTags;
		std::map<int, Core::String> _stringTags;

	public:
		Control();
		virtual ~Control();

		Style& getStyle() { return _style; }

		void setVisible(bool value) { _visible = value; }
		bool getVisible() { return _visible; }

		void setEnabled(bool value) { _style.enabled = value; }
		bool getEnabled() { return _style.enabled; }

		void setObjectTag(int key, void* value);
		void* getObjectTag(int key);

		void setStringTag(int key, Core::String value);
		Core::String getStringTag(int key);

		virtual int getControlType() = 0;

		virtual void update() = 0;
	};
}