#pragma once

#include <functional>

#include <Core/Shared/String.h>

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
        Core::String _text = "";

        Core::Texture* _image = nullptr;

        bool _active = true;

        std::function<void()> _onClick = nullptr;

      public:
        Button();
        Button(Core::String text);
        virtual ~Button();

        virtual int getControlType();
        virtual void update();

        Core::String getText() { return _text; }
        void setText(Core::String value) { _text = value; }

        void setImage(Core::Texture* value) { _image = value; }
        Core::Texture* getImage() { return _image; }

        bool getActive() { return _active; }
        void setActive(bool value);

        void setOnClick(std::function<void()> callback) { _onClick = callback; }
    };
} // namespace Editor