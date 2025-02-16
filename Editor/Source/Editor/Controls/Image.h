#pragma once

#include <Config.h>

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
		UInt32 nativeTextureId = 0;

		UInt32 width = 0;
		UInt32 height = 0;

	public:
		Image();
		Image(UInt32 width, UInt32 height);
		virtual ~Image();

		virtual void update();

		void setWidth(UInt32 value) { width = value; }
		UInt32 getWidth() { return width; }

		void setHeight(UInt32 value) { height = value; }
		UInt32 getHeight() { return height; }

		void setTexture(Core::Texture* value) { texture = value; }
		Core::Texture* getTexture() { return texture; }

		void setNativeTextureId(UInt32 value) { nativeTextureId = value; }
		UInt32 getNativeTextureId() { return nativeTextureId; }
	};
}