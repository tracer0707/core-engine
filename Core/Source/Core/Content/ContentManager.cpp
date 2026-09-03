#include "ContentManager.h"

#include <fstream>
#include <filesystem>
#include <map>

#include "../Shared/Path.h"
#include "../System/Application.h"
#include "../System/Window.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/VertexBuffer.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"
#include "../Interface/Transform.h"
#include "../Scene/Object.h"

#include "ContentDatabase.h"
#include "Material.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "RenderTexture.h"
#include "Scene.h"

#include "../Serialization/FlatBuffers/Content_generated.h"
#include "../Serialization/FlatBuffers/Scene_generated.h"

namespace fs = std::filesystem;

namespace Core
{
	ContentManager::ContentManager(Application* app, Renderer* renderer)
	{
		_app = app;
		_renderer = renderer;

		fs::path dbPath = _app->getRootPath() / "ContentDatabase.json";
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

	Mesh* ContentManager::createMesh()
	{
		Mesh* _mesh = new Mesh(_renderer);
		_meshes.add(_mesh);
		return _mesh;
	}

	RenderTexture* ContentManager::createRenderTexture(unsigned int width, unsigned int height)
	{
		RenderTexture* renderTexture = new RenderTexture(_renderer, width, height);
		_renderTextures.add(renderTexture);
		return renderTexture;
	}

	Scene* ContentManager::createScene()
	{
		Scene* _scene = new Scene(_renderer, _app->getMainWindow()->getTime());
		_scenes.add(_scene);
		return _scene;
	}

	// Load from files

	Material* ContentManager::loadMaterialFromFile(const fs::path& fileName)
	{
		Uuid uuid = ContentDatabase::singleton()->getUuid(fileName);

		auto it = _materialsCache.find(uuid);
		if (it != _materialsCache.end()) return (Material*)it->second;

		std::ifstream file(fileName, std::ios::binary | std::ios::ate);
		if (!file.is_open()) return nullptr;

		const std::streamsize fileSize = file.tellg();
		if (fileSize <= 0) return nullptr;

		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> buffer(static_cast<size_t>(fileSize));
		if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize))
		{
			return nullptr;
		}

		auto materialSerialized = flatbuffers::GetRoot<Core::Serialization::Material>(buffer.data());

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
			if (entry->value() != nullptr)
			{
				Uuid uuid(entry->value()->low(), entry->value()->high());
				tex = loadTexture2DByUuid(uuid);
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

	Texture2D* ContentManager::loadTexture2DFromFile(const fs::path& fileName)
	{
		Uuid uuid = ContentDatabase::singleton()->getUuid(fileName);

		auto it = _textures2DCache.find(uuid);
		if (it != _textures2DCache.end()) return (Texture2D*)it->second;

		std::ifstream file(fileName, std::ios::binary | std::ios::ate);
		if (!file.is_open()) return nullptr;

		const std::streamsize fileSize = file.tellg();
		if (fileSize <= 0) return nullptr;

		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> buffer(static_cast<size_t>(fileSize));
		if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize))
		{
			return nullptr;
		}

		auto texture2DSerialized = flatbuffers::GetRoot<Core::Serialization::Texture>(buffer.data());

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

	Mesh* ContentManager::loadMeshFromFile(const fs::path& fileName)
	{
		Uuid uuid = ContentDatabase::singleton()->getUuid(fileName);

		auto it = _meshesCache.find(uuid);
		if (it != _meshesCache.end()) return (Mesh*)it->second;

		std::ifstream file(fileName, std::ios::binary | std::ios::ate);
		if (!file.is_open()) return nullptr;

		const std::streamsize fileSize = file.tellg();
		if (fileSize <= 0) return nullptr;

		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> buffer(static_cast<size_t>(fileSize));
		if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize))
		{
			return nullptr;
		}

		Core::Mesh* result = new Mesh(_renderer);

		const Core::Serialization::Mesh* serializer = flatbuffers::GetRoot<Core::Serialization::Mesh>(buffer.data());

		const auto* serializedVertices = serializer->vertices();
		const uint32_t vertexCount = serializedVertices->size();

		std::vector<Core::Vertex> vertices;
		vertices.resize(vertexCount);

		for (uint32_t i = 0; i < vertexCount; ++i)
		{
			const Core::Serialization::Vertex* source = serializedVertices->Get(i);
			Core::Vertex& destination = vertices[i];

			destination._position = glm::vec3(source->position().x(), source->position().y(), source->position().z());
			destination._normal = glm::vec3(source->normal().x(), source->normal().y(), source->normal().z());
			destination._tangent = glm::vec3(source->tangent().x(), source->tangent().y(), source->tangent().z());
			destination._bitangent = glm::vec3(source->bitangent().x(), source->bitangent().y(), source->bitangent().z());
			destination._uv0 = glm::vec2(source->uv0().x(), source->uv0().y());
			destination._uv1 = glm::vec2(source->uv1().x(), source->uv1().y());
			destination._color = Color(source->color().x(), source->color().y(), source->color().z(), source->color().w());
			destination._blendWeights =
				glm::vec4(source->blend_weights().x(), source->blend_weights().y(), source->blend_weights().z(), source->blend_weights().w());
			destination._blendIndices =
				glm::vec4(source->blend_indices().x(), source->blend_indices().y(), source->blend_indices().z(), source->blend_indices().w());
		}

		const auto* serializedIndices = serializer->indices();
		const uint32_t indexCount = serializedIndices->size();

		std::vector<uint32_t> indices;
		indices.resize(indexCount);

		for (uint32_t i = 0; i < indexCount; ++i)
			indices[i] = serializedIndices->Get(i);

		result->updateVertexBuffer(vertices.data(), static_cast<unsigned int>(vertices.size()), indices.data(),
								   static_cast<unsigned int>(indices.size()));

		const auto* serializedSubMeshes = serializer->sub_meshes();

		if (serializedSubMeshes)
		{
			for (uint32_t i = 0; i < serializedSubMeshes->size(); ++i)
			{
				const Core::Serialization::SubMesh* source = serializedSubMeshes->Get(i);
				result->addSubMesh(source->index_offset(), source->index_count());
			}
		}

		const Core::Serialization::AABB* serializedAABB = serializer->aabb();

		if (serializedAABB)
		{
			const Core::Serialization::Vec3& min = serializedAABB->min();
			const Core::Serialization::Vec3& max = serializedAABB->max();

			result->setBoundingBox(Core::AxisAlignedBox(glm::vec3(min.x(), min.y(), min.z()), glm::vec3(max.x(), max.y(), max.z())));
		}

		result->setUuid(uuid);

		_meshes.add(result);
		_meshesCache[uuid] = result;

		if (_onResourceLoaded != nullptr)
		{
			_onResourceLoaded(result);
		}

		return result;
	}

	Scene* ContentManager::loadSceneFromFile(const fs::path& fileName)
	{
		Uuid uuid = ContentDatabase::singleton()->getUuid(fileName);

		auto it = _scenesCache.find(uuid);
		if (it != _scenesCache.end()) return (Scene*)it->second;

		std::ifstream file(fileName, std::ios::binary | std::ios::ate);
		if (!file.is_open()) return nullptr;

		const std::streamsize fileSize = file.tellg();
		if (fileSize <= 0) return nullptr;

		file.seekg(0, std::ios::beg);
		std::vector<uint8_t> buffer(static_cast<size_t>(fileSize));
		if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) return nullptr;

		const Core::Serialization::Scene* serializedScene = flatbuffers::GetRoot<Core::Serialization::Scene>(buffer.data());
		Scene* result = new Scene(_renderer, _app->getMainWindow()->getTime());
		result->setUuid(uuid);
		std::map<Uuid, Object*> objectsByUuid;
		std::map<Object*, Uuid> parentUuids;

		const auto* serializedObjects = serializedScene->objects();
		if (serializedObjects != nullptr)
		{
			for (const Core::Serialization::Object* serializedObject : *serializedObjects)
			{
				Object* object = result->createObject();
				if (serializedObject->name() != nullptr) object->setName(serializedObject->name()->c_str());
				if (serializedObject->uuid() != nullptr)
					object->setUuid(Uuid(serializedObject->uuid()->low(), serializedObject->uuid()->high()));
				objectsByUuid[object->getUuid()] = object;

				if (serializedObject->parent_uuid() != nullptr)
					parentUuids[object] = Uuid(serializedObject->parent_uuid()->low(), serializedObject->parent_uuid()->high());

				if (serializedObject->position() != nullptr)
				{
					const Core::Serialization::Vec3* position = serializedObject->position();
					object->getTransform()->setLocalPosition(glm::vec3(position->x(), position->y(), position->z()));
				}
				if (serializedObject->rotation() != nullptr)
				{
					const Core::Serialization::Vec4* rotation = serializedObject->rotation();
					object->getTransform()->setLocalRotation(glm::quat(rotation->w(), rotation->x(), rotation->y(), rotation->z()));
				}
				if (serializedObject->scale() != nullptr)
				{
					const Core::Serialization::Vec3* scale = serializedObject->scale();
					object->getTransform()->setLocalScale(glm::vec3(scale->x(), scale->y(), scale->z()));
				}

				const auto* serializedComponents = serializedObject->components();
				if (serializedComponents == nullptr) continue;

				for (const Core::Serialization::Component* serializedComponent : *serializedComponents)
				{
					if (serializedComponent->data_type() == Core::Serialization::ComponentData_Camera)
					{
						const Core::Serialization::Camera* serializedCamera = serializedComponent->data_as_Camera();
						if (serializedCamera == nullptr) continue;

						Camera* camera = object->addComponent<Camera*>();
						camera->setFov(serializedCamera->fov());
						camera->setNear(serializedCamera->near());
						camera->setFar(serializedCamera->far());
						if (result->getMainCamera() == nullptr) result->setMainCamera(camera);
					}
					else if (serializedComponent->data_type() == Core::Serialization::ComponentData_MeshRenderer)
					{
						const Core::Serialization::MeshRenderer* serializedMeshRenderer = serializedComponent->data_as_MeshRenderer();
						if (serializedMeshRenderer == nullptr || serializedMeshRenderer->mesh() == nullptr) continue;

						Uuid meshUuid(serializedMeshRenderer->mesh()->low(), serializedMeshRenderer->mesh()->high());
						if (meshUuid == Uuid::Empty) continue;

						Mesh* mesh = loadMeshByUuid(meshUuid);
						if (mesh == nullptr) continue;

						MeshRenderer* meshRenderer = object->addComponent<MeshRenderer*>();
						meshRenderer->setMesh(mesh);

						const auto* serializedMaterials = serializedMeshRenderer->materials();
						if (serializedMaterials != nullptr)
						{
							for (uint32_t i = 0; i < serializedMaterials->size() && i < static_cast<uint32_t>(meshRenderer->getMaterialCount()); ++i)
							{
								const Core::Serialization::Uuid* materialUuid = serializedMaterials->Get(i);
								if (materialUuid == nullptr) continue;

								Uuid uuidValue(materialUuid->low(), materialUuid->high());
								if (uuidValue != Uuid::Empty) meshRenderer->setMaterial(static_cast<int>(i), loadMaterialByUuid(uuidValue));
							}
						}
					}
				}
			}
		}

		for (const auto& [object, parentUuid] : parentUuids)
		{
			if (parentUuid == Uuid::Empty) continue;

			auto parent = objectsByUuid.find(parentUuid);
			if (parent != objectsByUuid.end()) object->getTransform()->setParent(parent->second->getTransform());
		}

		_scenes.add(result);
		_scenesCache[uuid] = result;

		if (_onResourceLoaded != nullptr) _onResourceLoaded(result);

		return result;
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

	Mesh* ContentManager::loadMeshByUuid(Uuid uuid)
	{
		ContentDatabase* db = ContentDatabase::singleton();
		if (!db->hasPath(uuid)) throw std::runtime_error("Resource not found");
		return loadMeshFromFile(db->getPath(uuid));
	}

	Scene* ContentManager::loadSceneByUuid(Uuid uuid)
	{
		ContentDatabase* db = ContentDatabase::singleton();
		if (!db->hasPath(uuid)) throw std::runtime_error("Resource not found");
		return loadSceneFromFile(db->getPath(uuid));
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

	void ContentManager::destroy(Scene* value)
	{
		destroyContent(value, _scenes);
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