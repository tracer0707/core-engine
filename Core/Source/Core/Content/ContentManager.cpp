#include "ContentManager.h"

#include <fstream>

#include "../Shared/Path.h"
#include "../System/Application.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/VertexBuffer.h"

#include "ContentDatabase.h"
#include "Material.h"
#include "Texture2D.h"
#include "Mesh.h"
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
	}

	ContentManager::~ContentManager()
	{
		for (auto it : _materials)
			delete it;
		for (auto it : _textures)
			delete it;
		for (auto it : _meshes)
			delete it;
		for (auto it : _renderTextures)
			delete it;

		_materials.clear();
		_textures.clear();
		_meshes.clear();
		_renderTextures.clear();

		_materialsCache.clear();
		_textures2DCache.clear();
		_meshesCache.clear();

		_renderer = nullptr;
	}

	// Create in memory

	Material* ContentManager::createMaterial()
	{
		Material* value = new Material(_renderer);
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

		Material* result = new Material(_renderer);
		result->setUuid(uuid);

		String programName = materialSerialized->program_name()->c_str();
		Program* program = _renderer->getShaderProgram(programName);
		result->setProgram(program);

		for (auto entry : *materialSerialized->int_values())
		{
			result->setInt(entry->key(), entry->value());
		}

		for (auto entry : *materialSerialized->float_values())
		{
			result->setFloat(entry->key(), entry->value());
		}

		for (auto entry : *materialSerialized->vec2_values())
		{
			auto value = entry->value();
			result->setVec2(entry->key(), glm::vec2(value->x(), value->y()));
		}

		for (auto entry : *materialSerialized->vec3_values())
		{
			auto value = entry->value();
			result->setVec3(entry->key(), glm::vec3(value->x(), value->y(), value->z()));
		}

		for (auto entry : *materialSerialized->vec4_values())
		{
			auto value = entry->value();
			result->setVec4(entry->key(), glm::vec4(value->x(), value->y(), value->z(), value->w()));
		}

		for (auto entry : *materialSerialized->texture2d_values())
		{
			Texture2D* tex = nullptr;
			String textureUuid = entry->value()->str();
			if (textureUuid != String::Empty)
			{
				tex = loadTexture2DByUuid(Core::Uuid::fromString(textureUuid.std_str()));
			}
			result->setTexture2D(entry->key(), tex);
		}

		_materials.add(result);
		_materialsCache[uuid] = result;

		if (_onResourceLoaded != nullptr)
		{
			_onResourceLoaded(result);
		}

		return result;
	}

	Texture2D* ContentManager::loadTexture2DFromFile(String fileName)
	{
		Uuid uuid = ContentDatabase::singleton()->getUuid(fileName);

		auto it = _textures2DCache.find(uuid);
		if (it != _textures2DCache.end()) return (Texture2D*)it->second;

		std::ifstream file(fileName.std_str(), std::ios::binary | std::ios::ate);
		size_t fileSize = file.tellg();
		file.seekg(0);

		std::vector<uint8_t> data(fileSize);
		file.read(reinterpret_cast<char*>(data.data()), fileSize);

		auto texture2DSerialized = GetTextureSerializer(data.data());

		unsigned char* dataRaw = const_cast<unsigned char*>(texture2DSerialized->data()->data());

		Texture2D* result = new Texture2D(_renderer, texture2DSerialized->width(), texture2DSerialized->height(), dataRaw,
										  texture2DSerialized->size(), static_cast<TextureFormat>(texture2DSerialized->format()));

		result->setUuid(uuid);

		_textures.add(result);
		_textures2DCache[uuid] = result;

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

	Texture2D* ContentManager::loadTexture2DByUuid(Uuid uuid)
	{
		ContentDatabase* db = ContentDatabase::singleton();
		if (!db->hasPath(uuid)) throw std::runtime_error("Resource not found");
		return loadTexture2DFromFile(db->getPath(uuid));
	}

	// Load from memory

	Texture2D* ContentManager::loadTexture2DFromBytes(unsigned char* data, int w, int h, int size, TextureFormat fmt)
	{
		Texture2D* result = new Texture2D(_renderer, w, h, data, size, fmt);
		_textures.add(result);
		return result;
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

	void ContentManager::destroy(Texture2D* value)
	{
		removeFromCache(value, _textures2DCache);
		destroyContent(value, _textures);
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