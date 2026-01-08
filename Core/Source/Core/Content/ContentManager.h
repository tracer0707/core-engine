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
	class ContentDatabase;
	class Application;

	class ContentManager
	{
		private:
			Renderer* _renderer = nullptr;
			Material* _defaultMaterial = nullptr;
			Shader* _defaultShader = nullptr;

			Application* _app = nullptr;
			ContentDatabase* _db = nullptr;

			List<Content*> _materials;
			List<Content*> _textures;
			List<Content*> _meshes;
			List<Content*> _shaders;
			List<Content*> _renderTextures;

			void destroyContent(Content* value, List<Content*>& _list);

		public:
			ContentManager(Application* app, Renderer* renderer);
			~ContentManager();

			Renderer* getRenderer() const { return _renderer; }

			Material* getDefaultMaterial() const { return _defaultMaterial; }
			Shader* getDefaultShader() const { return _defaultShader; }

			ContentDatabase* getContentDatabase() const { return _db; }

			Material* createMaterial();
			Mesh* createMesh(int subMeshesCount);
			RenderTexture* createRenderTexture(unsigned int width, unsigned int height);

			Material* loadMaterialFromFile(String fileName);
			Texture* loadTextureFromFile(String fileName);
			Texture* loadTextureFromBytes(unsigned char* data, int w, int h, int size, TextureFormat fmt);
			Mesh* loadMeshFromFile(String fileName);
			Shader* loadShaderFromString(String vertexSrc, String fragmentSrc);

			void destroy(Material* value);
			void destroy(Mesh* value);
			void destroy(Texture* value);
			void destroy(Shader* value);
			void destroy(RenderTexture* value);
	};
} // namespace Core