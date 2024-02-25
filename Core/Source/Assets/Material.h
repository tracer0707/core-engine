#pragma once

namespace Core
{
	class Texture;
	class Shader;

	class Material
	{
	private:
		Texture* _texture = nullptr;
		static Shader* _shader;

	public:
		Material();
		~Material();

		Texture* getTexture() { return _texture; }
		void setTexture(Texture* value) { _texture = value; }

		Shader* getShader() { return _shader; }
		void setShader(Shader* value) { _shader = value; }

		void bind();
	};
}