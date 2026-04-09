#pragma once

#include "Content.h"

namespace Core
{
	class Renderer;
	class Shader;

	class Material : public Content
	{
			friend class Renderer;
			friend class ContentManager;

		private:
			Renderer* _renderer = nullptr;
			Shader* _shader = nullptr;

			Material(Renderer* renderer);
			virtual ~Material();

		public:
			virtual int getContentType();

			const Shader* getShader() const { return _shader; }
			void setShader(Shader* value) { _shader = value; }

			void bind();
	};
} // namespace Core