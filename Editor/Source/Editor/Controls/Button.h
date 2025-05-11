#pragma once

#include <functional>

#include <Shared/String.h>
#include <Shared/Uuid.h>

#include "Control.h"

namespace Core
{
	class Texture;
}

namespace Editor
{
	class Button : public Control
	{
	private:
		UString _text = "";
		Core::Uuid _id;
		std::string _idStr = "";

		Core::Texture* _image = nullptr;
		
		int _width = 0;
		int _height = 0;

		bool _active = true;

		std::function<void()> _onClick = nullptr;

	public:
		Button();
		Button(UString text);
		virtual ~Button();

		virtual void update();

		UString getText() { return _text; }
		void setText(UString value) { _text = value; }

		void setImage(Core::Texture* value) { _image = value;  }
		Core::Texture* getImage() { return _image;  }

		void setWidth(int value) { _width = value; }
		int getWidth() { return _width; }

		void setHeight(int value) { _height = value; }
		int getHeight() { return _height; }

		void setSize(int width, int height) { _width = width; _height = height; }

		bool getActive() { return _active; }
		void setActive(bool value);

		void setOnClick(std::function<void()> callback) { _onClick = callback; }
	};
}