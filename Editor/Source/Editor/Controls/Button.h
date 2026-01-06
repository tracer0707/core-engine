#pragma once

#include <functional>

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

#include "Control.h"

namespace Core
{
	class Texture;
}

namespace Editor
{
	class ContextMenu;

	class Button : public Control
	{
		private:
			Core::String _text = "";

			Core::Texture* _image = nullptr;

			bool _active = true;
			bool _edit = false;
			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

			ContextMenu* _contextMenu = nullptr;

			std::function<void()> _onClick = nullptr;
			std::function<void()> _onDblClick = nullptr;
			std::function<void()> _onEditComplete = nullptr;
			std::function<void()> _onEditCancelled = nullptr;

		public:
			Button();
			Button(Core::String text);
			Button(Core::Texture* image);
			Button(Core::String text, Core::Texture* image);
			virtual ~Button();

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual int getControlType() const;
			virtual void update();

			Core::String getText() const { return _text; }
			void setText(Core::String value) { _text = value; }

			void setImage(Core::Texture* value) { _image = value; }
			Core::Texture* getImage() { return _image; }

			bool getActive() const { return _active; }
			void setActive(bool value);

			bool getUseContextMenu() const { return _contextMenu != nullptr; }
			void setUseContextMenu(bool value);

			ContextMenu* getContextMenu() const { return _contextMenu; }

			void startEdit() { _edit = true; }

			void setOnClick(std::function<void()> callback) { _onClick = callback; }
			void setOnDoubleClick(std::function<void()> callback) { _onDblClick = callback; }
			void setOnEditComplete(std::function<void()> callback) { _onEditComplete = callback; }
			void setOnEditCancelled(std::function<void()> callback) { _onEditCancelled = callback; }
	};
} // namespace Editor