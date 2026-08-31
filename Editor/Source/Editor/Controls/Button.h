#pragma once

#include <functional>

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

#include "Control.h"

namespace Editor
{
	class Texture;
	class ContextMenu;

	enum class ButtonType
	{
		Normal,
		Action,
	};

	class Button : public Control
	{
		private:
			Core::String _text = "";

			Texture* _image = nullptr;

			bool _active = true;
			bool _edit = false;
			float _imgW = 24.0f;
			float _imgH = 24.0f;

			ButtonType _buttonType = ButtonType::Normal;

			ContextMenu* _contextMenu = nullptr;

			std::function<void()> _onClick = nullptr;

		public:
			Button();
			Button(Core::String text);
			Button(Texture* image);
			Button(Core::String text, Texture* image);
			virtual ~Button();

			virtual ControlType getControlType() const { return ControlType::Button; }
			virtual void measure() const;
			virtual void update();

			Core::String getText() const { return _text; }
			void setText(Core::String value) { _text = value; }

			void setImage(Texture* value) { _image = value; }
			Texture* getImage() { return _image; }

			float getImageWidth() const { return _imgW; }
			void setImageWidth(float value) { _imgW = value; }

			float getImageHeight() const { return _imgH; }
			void setImageHeight(float value) { _imgH = value; }

			ButtonType getButtonType() const { return _buttonType; }
			void setButtonType(ButtonType value) { _buttonType = value; }

			bool getActive() const { return _active; }
			void setActive(bool value);

			bool getUseContextMenu() const { return _contextMenu != nullptr; }
			void setUseContextMenu(bool value);

			ContextMenu* getContextMenu() const { return _contextMenu; }

			void setOnClick(std::function<void()> callback) { _onClick = callback; }
	};
} // namespace Editor