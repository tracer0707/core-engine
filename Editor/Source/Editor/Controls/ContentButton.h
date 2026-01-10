#pragma once

#include <functional>

#include <Core/Shared/Uuid.h>
#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

#include "Control.h"

namespace Core
{
	class Content;
	class Texture;
}

namespace Editor
{
	class ContextMenu;

	class ContentButton : public Control
	{
		private:
			Core::Content* _content = nullptr;
			Core::Texture* _image = nullptr;

			bool _active = true;
			bool _edit = false;
			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;
			Core::String _editValue = Core::String::Empty;

			ContextMenu* _contextMenu = nullptr;
			
			std::function<void()> _onClick = nullptr;
			std::function<void()> _onDblClick = nullptr;
			std::function<void()> _onEditCancelled = nullptr;
			std::function<void(Core::String)> _onEditComplete = nullptr;

			Core::String getContentName() const;

		public:
			ContentButton();
			ContentButton(Core::Texture* image);
			virtual ~ContentButton();

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual int getControlType() const;
			virtual void update();

			Core::Content* getContent() const { return _content; }
			void setContent(Core::Content* value) { _content = value; }

			Core::Texture* getImage() { return _image; }
			void setImage(Core::Texture* value) { _image = value; }

			bool getActive() const { return _active; }
			void setActive(bool value);

			bool getUseContextMenu() const { return _contextMenu != nullptr; }
			void setUseContextMenu(bool value);

			ContextMenu* getContextMenu() const { return _contextMenu; }

			void startEdit();

			void setOnClick(std::function<void()> callback) { _onClick = callback; }
			void setOnDoubleClick(std::function<void()> callback) { _onDblClick = callback; }
			void setOnEditCancelled(std::function<void()> callback) { _onEditCancelled = callback; }
			void setOnEditComplete(std::function<void(Core::String)> callback) { _onEditComplete = callback; }
	};
} // namespace Editor