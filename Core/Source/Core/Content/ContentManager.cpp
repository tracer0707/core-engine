#include "ContentManager.h"

#include <fstream>

#include "../Renderer/Renderer.h"
#include "../Renderer/VertexBuffer.h"

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

		_db = new ContentDatabase(_app);

		_defaultShader = loadShaderFromString("#version 400\n"
											  "layout (location = 0) in vec3 position;"
											  "layout (location = 1) in vec2 uv0;"
											  "layout (location = 2) in vec4 color0;"
											  "out vec2 f_uv0;"
											  "out vec4 f_color0;"
											  "uniform mat4 u_viewMtx;"
											  "uniform mat4 u_projMtx;"
											  "uniform mat4 u_modelMtx;"
											  "void main() {"
											  "  f_uv0 = uv0;"
											  "  f_color0 = color0;"
											  "  gl_Position = u_projMtx * u_viewMtx * u_modelMtx * vec4(position, 1.0);"
											  "}",
											  "#version 400\n"
											  "in vec2 f_uv0;"
											  "in vec4 f_color0;"
											  "out vec4 frag_colour;"
											  "uniform sampler2D u_diffuseTex;"
											  "void main() {"
											  "  vec4 out_color = texture2D(u_diffuseTex, f_uv0);"
											  "  frag_colour = vec4(f_color0.rgb, 1.0);"
											  "}");

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

		_defaultMaterial = nullptr;
		_defaultShader = nullptr;
		_renderer = nullptr;
	}

	Uuid ContentManager::getOrCreateUuid(Core::String path)
	{
		Uuid uuid;

		if (_db->hasUuid(path))
		{
			uuid = _db->getUuid(path);
		}
		else
		{
			uuid = Uuid::create();
			_db->setPath(uuid, path);
		}

		return uuid;
	}

	// Create in memory

	Material* ContentManager::createMaterial()
	{
		Material* value = new Material(_renderer);
		value->setShader(_defaultShader);
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
		Uuid uuid = getOrCreateUuid(fileName);

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
			// load texture by uuid
		}

		Material* result = createMaterial();
		result->setTexture(tex);
		result->setUuid(uuid);

		_materials.add(result);
		_materialsCache[uuid] = result;

		return result;
	}

	Texture* ContentManager::loadTextureFromFile(String fileName)
	{
		Uuid uuid = getOrCreateUuid(fileName);

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
		if (!_db->hasPath(uuid)) throw std::runtime_error("Resource not found");
		return loadMaterialFromFile(_db->getPath(uuid));
	}

	Texture* ContentManager::loadTextureByUuid(Uuid uuid)
	{
		if (!_db->hasPath(uuid)) throw std::runtime_error("Resource not found");
		return loadTextureFromFile(_db->getPath(uuid));
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
		auto it = std::find_if(map.begin(), map.end(),
							   [value](const std::pair<Uuid, Content*>& pair) { return pair.second == value; });

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