#pragma once

#include <functional>

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

#include "Control.h"

namespace Core
{
	class Texture2D;
}

namespace Editor
{
	class ContextMenu;

	class Button : public Control
	{
		private:
			Core::String _text = "";

			Core::Texture2D* _image = nullptr;

			bool _active = true;
			bool _edit = false;
			float _imgW = 24.0f;
			float _imgH = 24.0f;

			ContextMenu* _contextMenu = nullptr;

			std::function<void()> _onClick = nullptr;

		public:
			Button();
			Button(Core::String text);
			Button(Core::Texture2D* image);
			Button(Core::String text, Core::Texture2D* image);
			virtual ~Button();

			virtual ControlType getControlType() const { return ControlType::Button; }
			virtual void update();

			Core::String getText() const { return _text; }
			void setText(Core::String value) { _text = value; }

			void setImage(Core::Texture2D* value) { _image = value; }
			Core::Texture2D* getImage() { return _image; }

			float getImageWidth() const { return _imgW; }
			void setImageWidth(float value) { _imgW = value; }

			float getImageHeight() const { return _imgH; }
			void setImageHeight(float value) { _imgH = value; }

			bool getActive() const { return _active; }
			void setActive(bool value);

			bool getUseContextMenu() const { return _contextMenu != nullptr; }
			void setUseContextMenu(bool value);

			ContextMenu* getContextMenu() const { return _contextMenu; }

			void setOnClick(std::function<void()> callback) { _onClick = callback; }
	};
} // namespace Editor