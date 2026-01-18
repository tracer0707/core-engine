#include "ContentManager.h"

#include <fstream>

#include "../Shared/Path.h"
#include "../System/Application.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/VertexBuffer.h"

#include "../Renderer/Shaders/GL4/ShaderDefaultUnlitColor.h"
#include "../Renderer/Shaders/GL4/ShaderDefaultUnlitTexture.h"

#include "ContentDatabase.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderTexture.h"

#include "../Serialization/FlatBuffers/TextureSerializer_generated.h"
#include "../Serialization/FlatBuffers/MaterialSerializer_generated.h"

namespace Core
{
	ContentManager::ContentManager(Application* app, Renderer* renderer)
	{
		_app = app;
		_renderer = renderer;

		String dbPath = Path::combine(_app->getRootPath(), "ContentDatabase.json");
		ContentDatabase* db = ContentDatabase::singleton();
		db->setApplication(app);
		db->setFilePath(dbPath);
		db->load();

		_defaultShaderUnlitColor = loadShaderFromString(ShaderDefaultUnlitColor::vertex.c_str(), ShaderDefaultUnlitColor::fragment.c_str());
		_defaultShaderUnlitTexture = loadShaderFromString(ShaderDefaultUnlitTexture::vertex.c_str(), ShaderDefaultUnlitTexture::fragment.c_str());

		_defaultMaterial = createMaterial();
	}

	ContentManager::~ContentManager()
	{
		for (auto it : _materials)
			delete it;
		for (auto it : _shaders)
			delete it;
		for (auto it : _textures)
			delete it;
		for (auto it : _meshes)
			delete it;
		for (auto it : _renderTextures)
			delete it;

		_materials.clear();
		_shaders.clear();
		_textures.clear();
		_meshes.clear();
		_renderTextures.clear();

		_renderer = nullptr;
		_defaultMaterial = nullptr;
		_defaultShaderUnlitColor = nullptr;
		_defaultShaderUnlitTexture = nullptr;
	}

	// Create in memory

	Material* ContentManager::createMaterial()
	{
		Material* value = new Material(_renderer);
		value->setShader(_defaultShaderUnlitColor);
		_materials.add(value);
		return value;
	}

	Mesh* ContentManager::createMesh(int subMeshesCount)
	{
		SubMesh** _subMeshes = new SubMesh*[subMeshesCount];
		Mesh* _mesh = new Mesh(_subMeshes, subMeshesCount);

		for (int i = 0; i < subMeshesCount; ++i)
		{
			_subMeshes[i] = new SubMesh(_renderer);
		}

		_meshes.add(_mesh);
		return _mesh;
	}

	RenderTexture* ContentManager::createRenderTexture(unsigned int width, unsigned int height)
	{
		RenderTexture* renderTexture = new RenderTexture(_renderer, width, height);
		_renderTextures.add(renderTexture);
		return renderTexture;
	}

	// Load from files

	Material* ContentManager::loadMaterialFromFile(String fileName)
	{
		Uuid uuid = ContentDatabase::singleton()->getUuid(fileName);

		auto it = _materialsCache.find(uuid);
		if (it != _materialsCache.end()) return (Material*)it->second;

		std::ifstream file(fileName.std_str(), std::ios::binary | std::ios::ate);
		size_t fileSize = file.tellg();
		file.seekg(0);

		std::vector<uint8_t> data(fileSize);
		file.read(reinterpret_cast<char*>(data.data()), fileSize);

		auto materialSerialized = GetMaterialSerializer(data.data());

		Texture* tex = nullptr;
		String textureUuid = materialSerialized->texture_uuid()->c_str();
		if (textureUuid != String::Empty)
		{
			tex = loadTextureByUuid(Core::Uuid::fromString(textureUuid.std_str()));
		}

		Material* result = new Material(_renderer);
		result->setShader(_defaultShaderUnlitColor);
		result->setTexture(tex);
		result->setUuid(uuid);

		_materials.add(result);
		_materialsCache[uuid] = result;

		if (_onResourceLoaded != nullptr)
		{
			_onResourceLoaded(result);
		}

		return result;
	}

	Texture* ContentManager::loadTextureFromFile(String fileName)
	{
		Uuid uuid = ContentDatabase::singleton()->getUuid(fileName);

		auto it = _texturesCache.find(uuid);
		if (it != _texturesCache.end()) return (Texture*)it->second;

		std::ifstream file(fileName.std_str(), std::ios::binary | std::ios::ate);
		size_t fileSize = file.tellg();
		file.seekg(0);

		std::vector<uint8_t> data(fileSize);
		file.read(reinterpret_cast<char*>(data.data()), fileSize);

		auto textureSerialized = GetTextureSerializer(data.data());

		unsigned char* dataRaw = const_cast<unsigned char*>(textureSerialized->data()->data());

		Texture* result = new Texture(_renderer, textureSerialized->width(), textureSerialized->height(), dataRaw, textureSerialized->size(),
									  static_cast<TextureFormat>(textureSerialized->format()));

		result->setUuid(uuid);

		_textures.add(result);
		_texturesCache[uuid] = result;

		if (_onResourceLoaded != nullptr)
		{
			_onResourceLoaded(result);
		}

		return result;
	}

	Mesh* ContentManager::loadMeshFromFile(String fileName)
	{
		// TODO
		return nullptr;
	}

	// Load by uuids

	Material* ContentManager::loadMaterialByUuid(Uuid uuid)
	{
		ContentDatabase* db = ContentDatabase::singleton();
		if (!db->hasPath(uuid)) throw std::runtime_error("Resource not found");
		return loadMaterialFromFile(db->getPath(uuid));
	}

	Texture* ContentManager::loadTextureByUuid(Uuid uuid)
	{
		ContentDatabase* db = ContentDatabase::singleton();
		if (!db->hasPath(uuid)) throw std::runtime_error("Resource not found");
		return loadTextureFromFile(db->getPath(uuid));
	}

	// Load from memory

	Texture* ContentManager::loadTextureFromBytes(unsigned char* data, int w, int h, int size, TextureFormat fmt)
	{
		Texture* result = new Texture(_renderer, w, h, data, size, fmt);
		_textures.add(result);
		return result;
	}

	Shader* ContentManager::loadShaderFromString(String vertexSrc, String fragmentSrc)
	{
		Shader* shader = new Shader(_renderer, vertexSrc, fragmentSrc);
		_shaders.add(shader);
		return shader;
	}

	// Destroy

	void ContentManager::destroy(Material* value)
	{
		removeFromCache(value, _materialsCache);
		destroyContent(value, _materials);
	}

	void ContentManager::destroy(Mesh* value)
	{
		removeFromCache(value, _meshesCache);
		destroyContent(value, _meshes);
	}

	void ContentManager::destroy(Texture* value)
	{
		removeFromCache(value, _texturesCache);
		destroyContent(value, _textures);
	}

	void ContentManager::destroy(Shader* value)
	{
		destroyContent(value, _shaders);
	}

	void ContentManager::destroy(RenderTexture* value)
	{
		destroyContent(value, _renderTextures);
	}

	void ContentManager::removeFromCache(Content* value, std::map<Uuid, Content*>& map)
	{
		auto it = std::find_if(map.begin(), map.end(), [value](const std::pair<Uuid, Content*>& pair) { return pair.second == value; });

		if (it != map.end())
		{
			map.erase(it);
		}
	}

	void ContentManager::destroyContent(Content* value, List<Content*>& list)
	{
		if (list.contains(value))
		{
			list.remove(value);
		}

		delete value;
	}
} // namespace Core