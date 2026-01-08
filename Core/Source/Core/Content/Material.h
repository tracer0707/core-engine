#pragma once

#include "Content.h"

namespace Core
{
	class Renderer;
	class Texture;
	class Shader;

	class Material : public Content
	{
			friend class Renderer;
			friend class ContentManager;

		private:
			Renderer* _renderer = nullptr;
			Texture* _texture = nullptr;
			Shader* _shader = nullptr;

			Material(Renderer* renderer);
			virtual ~Material();

		public:
			virtual int getContentType();

			Texture* getTexture() const { return _texture; }
			void setTexture(Texture* value) { _texture = value; }

			Shader* getShader() const { return _shader; }
			void setShader(Shader* value) { _shader = value; }

			void bind();
	};
} // namespace Core