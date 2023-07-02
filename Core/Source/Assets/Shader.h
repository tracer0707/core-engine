#pragma once

#include "../Renderer/Renderer.h"
#include "Asset.h"
#include "../Shared/String.h"

namespace Core
{
	class Shader : public Asset
	{
	private:
		Program nativeId;

	public:
		~Shader();

		virtual const void load();
		virtual const void unload();
		virtual const void loadFromString(UString vertexSrc, UString fragmentSrc);
		virtual const void bind();
	};
}