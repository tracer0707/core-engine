#pragma once

#include <string>
#include <map>

#include "Container.h"
#include "Style.h"

#include <Core/Shared/String.h>

namespace Editor
{
    class Window;
    class Container;

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

        float _width = 0;
        float _height = 0;

      public:
        Control();
        virtual ~Control();

        std::string getId() { return _id; }
        Style& getStyle() { return _style; }

        virtual void setWidth(float value) { _width = value; }
        virtual float getWidth() { return _width; }

        virtual void setHeight(float value) { _height = value; }
        virtual float getHeight() { return _height; }

        void setSize(float width, float height);

        void setVisible(bool value) { _visible = value; }
        bool getVisible() { return _visible; }

        void setEnabled(bool value) { _style.enabled = value; }
        bool getEnabled() { return _style.enabled; }

        void setObjectTag(int key, void* value);
        void* getObjectTag(int key);

        void setStringTag(int key, Core::String value);
        Core::String getStringTag(int key);

        virtual int getControlType() = 0;

        virtual void update() = 0;
    };
} // namespace Editor