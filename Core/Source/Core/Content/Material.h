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
		~Material();

	public:
		Texture* getTexture() { return _texture; }
		void setTexture(Texture* value) { _texture = value; }

		Shader* getShader() { return _shader; }
		void setShader(Shader* value) { _shader = value; }

		void bind();
	};
}