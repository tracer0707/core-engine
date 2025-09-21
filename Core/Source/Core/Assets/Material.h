#pragma once

#include "Asset.h"

namespace Core
{
	class Renderer;
	class Texture;
	class Shader;

	class Material : public Asset
	{
	friend class Renderer;
	friend class AssetManager;

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