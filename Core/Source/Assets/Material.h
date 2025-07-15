#pragma once

#include "Asset.h"

namespace Core
{
	class Texture;
	class Shader;

	class Material : public Asset
	{
	private:
		Texture* _texture = nullptr;
		Shader* _shader = nullptr;

	public:
		Material();
		~Material();

		static Material* defaultMaterial;

		Texture* getTexture() { return _texture; }
		void setTexture(Texture* value) { _texture = value; }

		Shader* getShader() { return _shader; }
		void setShader(Shader* value) { _shader = value; }

		void bind();
	};
}