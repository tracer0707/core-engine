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

		unsigned int width = 0;
		unsigned int height = 0;

	public:
		Image();
		Image(unsigned int width, unsigned int height);
		virtual ~Image();

		virtual void update();

		void setWidth(unsigned int value) { width = value; }
		unsigned int getWidth() { return width; }

		void setHeight(unsigned int value) { height = value; }
		unsigned int getHeight() { return height; }

		void setTexture(Core::Texture* value) { texture = value; }
		Core::Texture* getTexture() { return texture; }

		void setNativeTextureId(unsigned int value) { nativeTextureId = value; }
		unsigned int getNativeTextureId() { return nativeTextureId; }
	};
}