#pragma once

#include <map>
#include <functional>

#include "../Shared/Uuid.h"
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
	class Application;

	class ContentManager
	{
		private:
			Renderer* _renderer = nullptr;
			Material* _defaultMaterial = nullptr;
			Shader* _defaultShaderUnlitColor = nullptr;
			Shader* _defaultShaderUnlitTexture = nullptr;

			Application* _app = nullptr;

			std::function<void(Content*)> _onResourceLoaded = nullptr;

			List<Content*> _materials;
			List<Content*> _textures;
			List<Content*> _meshes;
			List<Content*> _shaders;
			List<Content*> _renderTextures;

			std::map<Uuid, Content*> _materialsCache;
			std::map<Uuid, Content*> _texturesCache;
			std::map<Uuid, Content*> _meshesCache;

			void removeFromCache(Content* value, std::map<Uuid, Content*>& map);
			void destroyContent(Content* value, List<Content*>& list);

		public:
			ContentManager(Application* app, Renderer* renderer);
			~ContentManager();

			Renderer* getRenderer() const { return _renderer; }

			Material* getDefaultMaterial() const { return _defaultMaterial; }
			Shader* getDefaultShaderUnlitColor() const { return _defaultShaderUnlitColor; }
			Shader* getDefaultShaderUnlitTexture() const { return _defaultShaderUnlitTexture; }

			void setOnResourceLoaded(std::function<void(Content*)> value) { _onResourceLoaded = value; }

			// Create in memory
			Material* createMaterial();
			Mesh* createMesh(int subMeshesCount);
			RenderTexture* createRenderTexture(unsigned int width, unsigned int height);

			// Load from files
			Material* loadMaterialFromFile(String fileName);
			Texture* loadTextureFromFile(String fileName);
			Mesh* loadMeshFromFile(String fileName);

			// Load by uuids
			Material* loadMaterialByUuid(Uuid uuid);
			Texture* loadTextureByUuid(Uuid uuid);

			// Load from memory
			Texture* loadTextureFromBytes(unsigned char* data, int w, int h, int size, TextureFormat fmt);
			Shader* loadShaderFromString(String vertexSrc, String fragmentSrc);

			// Destroy
			void destroy(Material* value);
			void destroy(Mesh* value);
			void destroy(Texture* value);
			void destroy(Shader* value);
			void destroy(RenderTexture* value);
	};
} // namespace Core