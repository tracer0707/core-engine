#pragma once

#include "Asset.h"

#include <map>

namespace Core
{
	class Renderer;
	class Texture;
	class Shader;

	class Material : public Asset
	{
	friend class Renderer;
	private:
		Texture* _texture = nullptr;
		Shader* _shader = nullptr;

		static std::map<Renderer*, Material*> defaultMaterials;

		static void destroyDefaultMaterial(Renderer* renderer);

	public:
		Material();
		~Material();

		Texture* getTexture() { return _texture; }
		void setTexture(Texture* value) { _texture = value; }

		Shader* getShader() { return _shader; }
		void setShader(Shader* value) { _shader = value; }

		static Material* getDefaultMaterial();

		void bind();
	};
}