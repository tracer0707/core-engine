#pragma once

#include <functional>

#include <Core/Shared/Uuid.h>
#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

#include "Control.h"

namespace Core
{
	class Content;
	class Texture2D;
} // namespace Core

namespace Editor
{
	class ContextMenu;
	class Texture;

	class ContentButton : public Control
	{
		private:
			Core::Content* _content = nullptr;
			Texture* _image = nullptr;
			Core::Texture2D* _coreImage = nullptr;

			bool _active = true;
			bool _edit = false;
			Core::String _editValue = Core::String::Empty;

			ContextMenu* _contextMenu = nullptr;

			std::function<void()> _onClick = nullptr;
			std::function<void()> _onDblClick = nullptr;
			std::function<void()> _onEditCancelled = nullptr;
			std::function<void(Core::String)> _onEditComplete = nullptr;

			Core::String getContentName() const;

		public:
			ContentButton();
			ContentButton(Texture* image);
			ContentButton(Core::Texture2D* coreImage);
			virtual ~ContentButton();

			virtual ControlType getControlType() const { return ControlType::ContentButton; }
			virtual void measure() const;
			virtual void update();

			Core::Content* getContent() const { return _content; }
			void setContent(Core::Content* value) { _content = value; }

			Texture* getImage() const { return _image; }
			void setImage(Texture* value) { _image = value; }

			Core::Texture2D* getCoreImage() const { return _coreImage; }
			void setCoreImage(Core::Texture2D* value) { _coreImage = value; }

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