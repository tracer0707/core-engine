#pragma once

#include "Control.h"

namespace Core
{
    class Texture;
}

namespace Editor
{
    class Image : public Control
    {
      private:
        Core::Texture* texture = nullptr;
        unsigned int nativeTextureId = 0;

      public:
        Image();
        Image(int width, int height);
        virtual ~Image();

        virtual int getControlType() const;
        virtual void update();

        void setTexture(Core::Texture* value) { texture = value; }
        Core::Texture* getTexture() { return texture; }

        void setNativeTextureId(unsigned int value) { nativeTextureId = value; }
        unsigned int getNativeTextureId() const { return nativeTextureId; }
    };
} // namespace Editor