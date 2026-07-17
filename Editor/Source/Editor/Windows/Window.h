#pragma once

#include <functional>
#include <string>
#include <cfloat>
#include <Core/Shared/Uuid.h>

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
	class WindowManager;

	class Window : public Container
	{
			friend class WindowManager;

		private:
			Core::Uuid _id;

			DockArea _dockArea;

			std::function<void(int, int)> _onResize = nullptr;

			std::string _name = "";

			float _positionX = 0;
			float _positionY = 0;
			float _fixedPositionX = FLT_MAX;
			float _fixedPositionY = FLT_MAX;
			float _width = 0;
			float _height = 0;
			float _fixedWidth = FLT_MAX;
			float _fixedHeight = FLT_MAX;
			float _clientWidth = 0;
			float _clientHeight = 0;

			bool _visible = true;
			bool _hasTitle = true;
			bool _hasDockTitle = true;
			bool _isHovered = false;
			bool _canAcceptDocking = true;
			bool _canDock = true;
			bool _canClose = true;
			bool _canResize = true;
			bool _canMove = true;

			void update();

		protected:
			Style _style;

			WindowManager* _parent = nullptr;

			virtual void onResize(int newWidth, int newHeight) {}
			virtual void onUpdate() {}
			virtual void onClose() {}

		public:
			Window(WindowManager* parent, std::string name);
			virtual ~Window();

			virtual void init() {}

			WindowManager* getParent() { return _parent; }
			std::string getName() const { return _name; };

			void setVisible(bool value) { _visible = value; }
			bool getVisible() const { return _visible; }

			float getWidth() const { return _width; }
			float getHeight() const { return _height; }

			void setWidth(float value) { _fixedWidth = value; }
			void setHeight(float value) { _fixedHeight = value; }

			float getClientWidth() const { return _clientWidth; }
			float getClientHeight() const { return _clientHeight; }

			float getPositionX() const { return _positionX; }
			float getPositionY() const { return _positionY; }

			void setPositionX(float value) { _fixedPositionX = value; }
			void setPositionY(float value) { _fixedPositionY = value; }

			bool getIsHovered() const { return _isHovered; }

			void setHasTitle(bool value) { _hasTitle = value; }
			bool getHasTitle() const { return _hasTitle; }

			void setHasDockTitle(bool value) { _hasDockTitle = value; }
			bool getHasDockTitle() const { return _hasDockTitle; }

			void setCanAcceptDocking(bool value) { _canAcceptDocking = value; }
			bool getCanAcceptDocking() const { return _canAcceptDocking; }

			void setCanDock(bool value) { _canDock = value; }
			bool getCanDock() const { return _canDock; }

			void setCanResize(bool value) { _canResize = value; }
			bool getCanResize() const { return _canResize; }

			void setCanMove(bool value) { _canMove = value; }
			bool getCanMove() const { return _canMove; }

			void setCanClose(bool value) { _canClose = value; }
			bool getCanClose() const { return _canClose; }

			void setOnResize(std::function<void(int, int)> value) { _onResize = value; }
			void close();

			Style& getStyle() { return _style; }

			Core::Uuid getId() { return _id; }

			DockArea dock(DockDirection dockDirection, unsigned int relativeTo, float splitSize);

			virtual void invalidate() {}
	};
} // namespace Editor