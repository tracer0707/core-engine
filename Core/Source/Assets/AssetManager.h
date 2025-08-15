#pragma once

#include "../Shared/String.h"
#include "../Shared/List.h"
#include "../Renderer/TextureFormat.h"

namespace Core
{
	class Renderer;
	class Material;
	class Texture;
	class Mesh;
	class Shader;
	class RenderTexture;

	class AssetManager
	{
	private:
		Renderer* _renderer = nullptr;
		Material* _defaultMaterial = nullptr;
		Shader* _defaultShader = nullptr;

		List<Material*> _materials;
		List<Texture*> _textures;
		List<Mesh*> _meshes;
		List<Shader*> _shaders;
		List<RenderTexture*> _renderTextures;

		void destroyMaterial(Material* value);
		void destroyTexture(Texture* value);
		void destroyMesh(Mesh* value);
		void destroyShader(Shader* value);
		void destroyRenderTexture(RenderTexture* value);

	public:
		AssetManager(Renderer* renderer);
		~AssetManager();

		Material* getDefaultMaterial() { return _defaultMaterial; }
		Shader* getDefaultShader() { return _defaultShader; }

		Material* createMaterial();
		Mesh* createMesh(int subMeshesCount);
		RenderTexture* createRenderTexture(unsigned int width, unsigned int height);

		Texture* loadTextureFromFile(UString fileName, TextureFormat fmt);
		Texture* loadTextureFromBytes(unsigned char* data, int w, int h, int bpp, TextureFormat fmt);
		Mesh* loadMeshFromFile(UString fileName);
		Shader* loadShaderFromString(const char* vertexSrc, const char* fragmentSrc);

		void destroy(Material* value) { destroyMaterial(value); }
		void destroy(Mesh* value) { destroyMesh(value); }
		void destroy(Texture* value) { destroyTexture(value); }
		void destroy(Shader* value) { destroyShader(value); }
		void destroy(RenderTexture* value) { destroyRenderTexture(value); }
	};
}