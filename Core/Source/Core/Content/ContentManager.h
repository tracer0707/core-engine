#pragma once

#include <map>
#include <functional>

#include "../Shared/Uuid.h"
#include "../Shared/String.h"
#include "../Shared/List.h"
#include "../Shared/Path.h"
#include "../Renderer/TextureFormat.h"

namespace Core
{
	class Content;
	class Renderer;
	class Material;
	class Texture2D;
	class Mesh;
	class RenderTexture;
	class Scene;
	class Script;
	class Application;

	class ContentManager
	{
		private:
			Renderer* _renderer = nullptr;

			Application* _app = nullptr;

			std::function<void(Content*)> _onResourceLoaded = nullptr;

			List<Content*> _materials;
			List<Content*> _textures;
			List<Content*> _meshes;
			List<Content*> _renderTextures;
			List<Content*> _scenes;
			List<Content*> _scripts;

			std::map<Uuid, Content*> _materialsCache;
			std::map<Uuid, Content*> _textures2DCache;
			std::map<Uuid, Content*> _meshesCache;
			std::map<Uuid, Content*> _renderTexturesCache;
			std::map<Uuid, Content*> _scenesCache;
			std::map<Uuid, Content*> _scriptsCache;

			void removeFromCache(Content* value, std::map<Uuid, Content*>& map);
			void destroyContent(Content* value, List<Content*>& list);

		public:
			ContentManager(Application* app, Renderer* renderer);
			~ContentManager();

			Renderer* getRenderer() const { return _renderer; }

			void setOnResourceLoaded(std::function<void(Content*)> value) { _onResourceLoaded = value; }

			// Create in memory
			Material* createMaterial();
			Mesh* createMesh();
			RenderTexture* createRenderTexture(unsigned int width, unsigned int height);
			Scene* createScene();

			// Load from files
			Material* loadMaterialFromFile(const fs::path& fileName);
			Texture2D* loadTexture2DFromFile(const fs::path& fileName);
			Mesh* loadMeshFromFile(const fs::path& fileName);
			Scene* loadSceneFromFile(const fs::path& fileName);
			Script* loadScriptFromFile(const fs::path& fileName);

			// Load by uuids
			Material* loadMaterialByUuid(Uuid uuid);
			Texture2D* loadTexture2DByUuid(Uuid uuid);
			Mesh* loadMeshByUuid(Uuid uuid);
			Scene* loadSceneByUuid(Uuid uuid);
			Script* loadScriptByUuid(Uuid uuid);

			// Load from memory
			Texture2D* loadTexture2DFromBytes(unsigned char* data, int w, int h, int size, TextureFormat fmt);

			// Destroy
			void destroy(Material* value);
			void destroy(Mesh* value);
			void destroy(Texture2D* value);
			void destroy(RenderTexture* value);
			void destroy(Scene* value);
			void destroy(Script* value);
	};
} // namespace Core