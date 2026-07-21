#pragma once

#include <string>
#include <map>
#include <functional>

#include "Container.h"
#include "Style.h"
#include "ControlType.h"

#include <Core/Shared/String.h>

namespace Editor
{
	class Window;
	class Container;

	struct DragDropData
	{
			Core::String key = Core::String::Empty;
			void* value = nullptr;
	};

	class Control : public Container
	{
			friend class Container;

		protected:
			std::string _id = "";
			Container* _parent = nullptr;
			Style _style;
			bool _visible = true;
			std::map<int, void*> _objectTags;
			std::map<int, Core::String> _stringTags;

			float _x = FLT_MIN;
			float _y = FLT_MIN;
			float _computedX = 0;
			float _computedY = 0;
			float _width = 0;
			float _height = 0;
			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

			Core::String _dragDropSourceKey = Core::String::Empty;
			Core::String _dragDropSourceLabel = Core::String::Empty;
			Core::String _dragDropTargetKey = Core::String::Empty;

			bool _isDragDropSource = false;
			bool _isDragDropTarget = false;
			DragDropData* _dragDropSourceData = nullptr;

			bool _isDragDropEnter = false;

			std::function<void(DragDropData*, int, int)> _onDragDrop = nullptr;
			std::function<void(DragDropData*, int, int)> _onDragEnter = nullptr;
			std::function<void(DragDropData*, int, int)> _onDragOver = nullptr;
			std::function<void(DragDropData*)> _onDragExit = nullptr;

			void updateDragDropSource();
			void updateDragDropTarget();

		public:
			Control();
			virtual ~Control();

			std::string getId() { return _id; }
			Style& getStyle() { return _style; }

			float getWidth() const;
			void setWidth(float value) { _width = value; }
			
			float getHeight() const;
			void setHeight(float value) { _height = value; }

			float getPositionX() const { return _computedX; }
			void setPositionX(float value) { _x = value; }

			float getPositionY() const { return _computedY; }
			void setPositionY(float value) { _y = value; }

			void setSize(float width, float height);
			void setPosition(float x, float y);

			void setVisible(bool value) { _visible = value; }
			bool getVisible() const { return _visible; }

			void setEnabled(bool value) { _style.enabled = value; }
			bool getEnabled() const { return _style.enabled; }

			void setObjectTag(int key, void* value);
			void* getObjectTag(int key) const;

			void setStringTag(int key, Core::String value);
			Core::String getStringTag(int key) const;

			void setDragDropSource(bool value, Core::String key);
			bool getIsDragDropSource() const { return _isDragDropSource; }

			void setDragDropSourceLabel(Core::String value) { _dragDropSourceLabel = value; }
			Core::String getDragDropSourceLabel() const { return _dragDropSourceLabel; }

			void setDragDropSourceData(DragDropData data);
			const DragDropData* getDragDropSourceData() const { return _dragDropSourceData; }

			void setDragDropTarget(bool value, Core::String key);
			bool getIsDragDropTarget() const { return _isDragDropTarget; }

			void setOnDragDrop(std::function<void(DragDropData*, int, int)> callback) { _onDragDrop = callback; }
			void setOnDragEnter(std::function<void(DragDropData*, int, int)> callback) { _onDragEnter = callback; }
			void setOnDragOver(std::function<void(DragDropData*, int, int)> callback) { _onDragOver = callback; }
			void setOnDragExit(std::function<void(DragDropData*)> callback) { _onDragExit = callback; }

			const std::map<int, void*>& getObjectTags() const { return _objectTags; }
			const std::map<int, Core::String>& getStringTags() const { return _stringTags; }

			virtual ControlType getControlType() const = 0;

			virtual void update() = 0;
	};
} // namespace Editor