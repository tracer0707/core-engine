#pragma once

#include "Control.h"

namespace Editor
{
	class Texture;

	class Image : public Control
	{
		private:
			Texture* _texture = nullptr;
			unsigned int _nativeTextureId = 0u;

		public:
			Image();
			Image(int width, int height);
			virtual ~Image();

			virtual ControlType getControlType() const { return ControlType::Image; }
			virtual void measure() const;
			virtual void update();

			void setTexture(Texture* value) { _texture = value; }
			Texture* getTexture() { return _texture; }

			void setNativeTextureId(unsigned int value) { _nativeTextureId = value; }
			unsigned int getNativeTextureId() const { return _nativeTextureId; }
	};
} // namespace Editor