#pragma once

#include "Control.h"

namespace Core
{
	class Texture2D;
}

namespace Editor
{
	class Image : public Control
	{
		private:
			Core::Texture2D* texture = nullptr;
			unsigned int nativeTextureId = 0;

		public:
			Image();
			Image(int width, int height);
			virtual ~Image();

			virtual ControlType getControlType() const { return ControlType::Image; }
			virtual void measure() const;
			virtual void update();

			void setTexture(Core::Texture2D* value) { texture = value; }
			Core::Texture2D* getTexture() { return texture; }

			void setNativeTextureId(unsigned int value) { nativeTextureId = value; }
			unsigned int getNativeTextureId() const { return nativeTextureId; }
	};
} // namespace Editor