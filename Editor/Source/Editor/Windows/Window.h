#pragma once

#include <functional>
#include <Shared/Uuid.h>

#include "../Controls/Container.h"
#include "../Controls/Style.h"

namespace Editor
{
	enum class DockDirection
	{
		None,
		Up,
		Down,
		Left,
		Right
	};

	class DockArea
	{
	public:
		unsigned int area1 = 0;
		unsigned int area2 = 0;
	};

	class Control;

	class Window: public Container
	{
		friend class WindowManager;

	private:
		Core::Uuid _id;

		DockArea _dockArea;

		std::function<void(int, int)> _onResize = nullptr;

		const char* _name = nullptr;

		float _positionX = 0;
		float _positionY = 0;
		float _width = 0;
		float _height = 0;
		float _clientWidth = 0;
		float _clientHeight = 0;

		bool _visible = true;
		bool _hasTitle = true;
		bool _canAcceptDocking = true;
		bool _canDock = true;
		bool _isHovered = false;

		void update();

	protected:
		Style _style;

		WindowManager* _parent = nullptr;

		virtual void onResize(int newWidth, int newHeight) {}
		virtual void onUpdate() {}

	public:
		Window(WindowManager* parent, const char* name);
		virtual ~Window();

		WindowManager* getParent() { return _parent; }

		const char* getName() { return _name; };

		void setVisible(bool value) { _visible = value; }
		bool getVisible() { return _visible; }

		float getWidth() { return _width; }
		float getHeight() { return _height; }

		float getClientWidth() { return _clientWidth; }
		float getClientHeight() { return _clientHeight; }

		float getPositionX() { return _positionX; }
		float getPositionY() { return _positionY; }

		bool getIsHovered() { return _isHovered; }

		void setHasTitle(bool value) { _hasTitle = value; }
		bool getHasTitle() { return _hasTitle; }

		void setCanAcceptDocking(bool value) { _canAcceptDocking = value; }
		bool getCanAcceptDocking() { return _canAcceptDocking; }

		void setCanDock(bool value) { _canDock = value; }
		bool getCanDock() { return _canDock; }

		void setOnResize(std::function<void(int, int)> value) { _onResize = value; }

		Style& getStyle() { return _style; }

		Core::Uuid getId() { return _id; }

		DockArea dock(DockDirection dockDirection, unsigned int relativeTo, float splitSize);

		virtual void invalidate() {}
	};
}