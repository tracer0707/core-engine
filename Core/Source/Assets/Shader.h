#pragma once

#include "../Renderer/Program.h"
#include "../Shared/String.h"

#include "Asset.h"

namespace Core
{
	class Renderer;

	class Shader : public Asset
	{
		friend class AssetManager;

	private:
		Shader(Renderer* renderer, UString vertexSrc, UString fragmentSrc);
		~Shader();

		Renderer* _renderer = nullptr;
		const Program* _nativeId = nullptr;

	public:
		void bind();
	};
}