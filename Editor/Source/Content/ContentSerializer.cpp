#include "ContentSerializer.h"

#include <fstream>

#include <Core/Renderer/Program.h>
#include <Core/Content/Material.h>
#include <Core/Content/Mesh.h>
#include <Core/Content/Scene.h>
#include <Core/Content/Texture2D.h>
#include <Core/Content/ContentDatabase.h>
#include <Core/Shared/Path.h>
#include <Core/Scene/Object.h>
#include <Core/Interface/Transform.h>
#include <Core/Components/Camera.h>
#include <Core/Components/MeshRenderer.h>

#include <Core/Serialization/FlatBuffers/Content_generated.h>
#include <Core/Serialization/FlatBuffers/Scene_generated.h>

namespace Editor
{
	void ContentSerializer::serializeMaterial(Core::Material* value)
	{
		fs::path path = Core::ContentDatabase::singleton()->getPath(value->getUuid());
		serializeMaterial(value, path);
	}

	void ContentSerializer::serializeMaterial(Core::Material* value, const fs::path& path)
	{
		flatbuffers::FlatBufferBuilder builder;

		std::vector<flatbuffers::Offset<Core::Serialization::MapEntryInt>> ints;
		ints.reserve(value->getIntValues().size());

		for (const auto& [key, value] : value->getIntValues())
		{
			ints.push_back(Core::Serialization::CreateMapEntryInt(builder, key, value));
		}

		auto intVector = builder.CreateVector(ints);

		std::vector<flatbuffers::Offset<Core::Serialization::MapEntryFloat>> floats;
		floats.reserve(value->getFloatValues().size());

		for (const auto& [key, value] : value->getFloatValues())
		{
			floats.push_back(Core::Serialization::CreateMapEntryFloat(builder, key, value));
		}

		auto floatVector = builder.CreateVector(floats);

		std::vector<flatbuffers::Offset<Core::Serialization::MapEntryVec2>> vec2s;
		vec2s.reserve(value->getVec2Values().size());

		for (const auto& [key, value] : value->getVec2Values())
		{
			Core::Serialization::Vec2 fbVec(value.x, value.y);
			vec2s.push_back(Core::Serialization::CreateMapEntryVec2(builder, key, &fbVec));
		}

		auto vec2Vector = builder.CreateVector(vec2s);

		std::vector<flatbuffers::Offset<Core::Serialization::MapEntryVec3>> vec3s;
		vec3s.reserve(value->getVec3Values().size());

		for (const auto& [key, value] : value->getVec3Values())
		{
			Core::Serialization::Vec3 fbVec(value.x, value.y, value.z);
			vec3s.push_back(Core::Serialization::CreateMapEntryVec3(builder, key, &fbVec));
		}

		auto vec3Vector = builder.CreateVector(vec3s);

		std::vector<flatbuffers::Offset<Core::Serialization::MapEntryVec4>> vec4s;
		vec4s.reserve(value->getVec4Values().size());

		for (const auto& [key, value] : value->getVec4Values())
		{
			Core::Serialization::Vec4 fbVec(value.x, value.y, value.z, value.w);
			vec4s.push_back(Core::Serialization::CreateMapEntryVec4(builder, key, &fbVec));
		}

		auto vec4Vector = builder.CreateVector(vec4s);

		std::vector<flatbuffers::Offset<Core::Serialization::MapEntryUuid>> tex2ds;
		tex2ds.reserve(value->getTexture2dValues().size());

		for (const auto& [key, value] : value->getTexture2dValues())
		{
			const Core::Uuid& uuid = value->getUuid();
			if (uuid == Core::Uuid::Empty)
			{
				tex2ds.push_back(Core::Serialization::CreateMapEntryUuid(builder, key, nullptr));
			}
			else
			{
				auto [low, high] = uuid.toUInt64();
				Core::Serialization::Uuid fbUuid(low, high);
				tex2ds.push_back(Core::Serialization::CreateMapEntryUuid(builder, key, &fbUuid));
			}
		}

		auto tex2dVector = builder.CreateVector(tex2ds);

		std::string programNameRaw = "";

		if (value->getProgram() != nullptr) programNameRaw = value->getProgram()->name.std_str();

		flatbuffers::Offset<flatbuffers::String> programName = builder.CreateString(programNameRaw);

		auto materialFB =
			Core::Serialization::CreateMaterial(builder, programName, intVector, floatVector, vec2Vector, vec3Vector, vec4Vector, tex2dVector);

		builder.Finish(materialFB);

		uint8_t* buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		std::ofstream file(path, std::ios::binary);
		file.write(reinterpret_cast<const char*>(buf), size);
		file.close();
	}
	
	void ContentSerializer::serializeScene(Core::Scene* value) 
	{
		fs::path path = Core::ContentDatabase::singleton()->getPath(value->getUuid());
		serializeScene(value, path);
	}
	
	void ContentSerializer::serializeScene(Core::Scene* value, const fs::path& path)
	{
		flatbuffers::FlatBufferBuilder builder;
		std::vector<flatbuffers::Offset<Core::Serialization::Object>> objects;
		objects.reserve(value->getObjects().count());

		for (Core::Object* object : value->getObjects())
		{
			Core::Transform* transform = object->getTransform();
			Core::Uuid parentUuid = Core::Uuid::Empty;
			if (transform->getParent() != nullptr)
				parentUuid = ((Core::Object*)transform->getParent()->getOwner())->getUuid();

			auto [parentLow, parentHigh] = parentUuid.toUInt64();
			Core::Serialization::Uuid fbParentUuid(parentLow, parentHigh);

			glm::vec3 position = transform->getLocalPosition();
			glm::quat rotation = transform->getLocalRotation();
			glm::vec3 scale = transform->getLocalScale();
			Core::Serialization::Vec3 fbPosition(position.x, position.y, position.z);
			Core::Serialization::Vec4 fbRotation(rotation.x, rotation.y, rotation.z, rotation.w);
			Core::Serialization::Vec3 fbScale(scale.x, scale.y, scale.z);

			std::vector<flatbuffers::Offset<Core::Serialization::Component>> components;
			components.reserve(object->getComponents().count());

			for (Core::Component* component : object->getComponents())
			{
				flatbuffers::Offset<void> data;
				Core::Serialization::ComponentData dataType = Core::Serialization::ComponentData_NONE;

				if (component->getComponentType() == Core::ComponentType::Camera)
				{
					Core::Camera* camera = static_cast<Core::Camera*>(component);
					data = Core::Serialization::CreateCamera(builder, camera->getFov(), camera->getNear(), camera->getFar()).Union();
					dataType = Core::Serialization::ComponentData_Camera;
				}
				else if (component->getComponentType() == Core::ComponentType::MeshRenderer)
				{
					Core::MeshRenderer* meshRenderer = static_cast<Core::MeshRenderer*>(component);
					Core::Uuid meshUuid = meshRenderer->getMesh() != nullptr ? meshRenderer->getMesh()->getUuid() : Core::Uuid::Empty;
					auto [meshLow, meshHigh] = meshUuid.toUInt64();
					Core::Serialization::Uuid fbMeshUuid(meshLow, meshHigh);

					std::vector<Core::Serialization::Uuid> materialUuids;
					materialUuids.reserve(meshRenderer->getMaterialCount());
					for (int i = 0; i < meshRenderer->getMaterialCount(); ++i)
					{
						Core::Material* material = meshRenderer->getMaterial(i);
						Core::Uuid materialUuid = material != nullptr ? material->getUuid() : Core::Uuid::Empty;
						auto [low, high] = materialUuid.toUInt64();
						materialUuids.emplace_back(low, high);
					}

					auto materials = builder.CreateVectorOfStructs(materialUuids);
					data = Core::Serialization::CreateMeshRenderer(builder, &fbMeshUuid, materials).Union();
					dataType = Core::Serialization::ComponentData_MeshRenderer;
				}

				if (dataType != Core::Serialization::ComponentData_NONE)
					components.push_back(Core::Serialization::CreateComponent(builder, dataType, data));
			}

			auto name = builder.CreateString(object->getName().std_str());
			auto [low, high] = object->getUuid().toUInt64();
			Core::Serialization::Uuid uuid(low, high);
			auto componentVector = builder.CreateVector(components);
			objects.push_back(Core::Serialization::CreateObject(builder, name, &uuid, &fbParentUuid, &fbPosition, &fbRotation, &fbScale, componentVector));
		}

		auto objectVector = builder.CreateVector(objects);
		builder.Finish(Core::Serialization::CreateScene(builder, objectVector));

		std::ofstream file(path, std::ios::binary);
		file.write(reinterpret_cast<const char*>(builder.GetBufferPointer()), builder.GetSize());
	}
} // namespace Editor