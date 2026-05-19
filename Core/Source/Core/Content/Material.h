#pragma once

#include "Content.h"

namespace Core
{
	class Renderer;
	class ShaderGraph;

	class Material : public Content
	{
			friend class Renderer;
			friend class ContentManager;

		private:
			Renderer* _renderer = nullptr;
			ShaderGraph* _shaderGraph = nullptr;

			Material(Renderer* renderer);
			virtual ~Material();

		public:
			virtual int getContentType();

			const ShaderGraph* getShaderGraph() const { return _shaderGraph; }
			void setShaderGraph(ShaderGraph* value) { _shaderGraph = value; }

			void bind();
	};
} // namespace Core