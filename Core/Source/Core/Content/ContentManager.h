#pragma once

#include "../Shared/String.h"
#include "../Shared/List.h"
#include "../Renderer/TextureFormat.h"

namespace Core
{
    class Content;
	class Renderer;
	class Material;
	class Texture;
	class Mesh;
	class Shader;
	class RenderTexture;

	class ContentManager
	{
	private:
		Renderer* _renderer = nullptr;
		Material* _defaultMaterial = nullptr;
		Shader* _defaultShader = nullptr;

		List<Content*> _materials;
        List<Content*> _textures;
        List<Content*> _meshes;
        List<Content*> _shaders;
        List<Content*> _renderTextures;

		void destroyContent(Content* value, List<Content*>& _list);

	public:
        ContentManager(Renderer* renderer);
        ~ContentManager();

		Renderer* getRenderer() { return _renderer; }

		Material* getDefaultMaterial() { return _defaultMaterial; }
		Shader* getDefaultShader() { return _defaultShader; }

		Material* createMaterial();
		Mesh* createMesh(int subMeshesCount);
		RenderTexture* createRenderTexture(unsigned int width, unsigned int height);

		Texture* loadTextureFromFile(String fileName, TextureFormat fmt);
		Texture* loadTextureFromBytes(unsigned char* data, int w, int h, int size, TextureFormat fmt);
		Mesh* loadMeshFromFile(String fileName);
		Shader* loadShaderFromString(String vertexSrc, String fragmentSrc);

		void destroy(Material* value);
        void destroy(Mesh* value);
        void destroy(Texture* value);
        void destroy(Shader* value);
        void destroy(RenderTexture* value);
	};
}