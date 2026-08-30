#include "ContentSerializer.h"

#include <fstream>

#include <Core/Renderer/Program.h>
#include <Core/Content/Material.h>
#include <Core/Content/Texture2D.h>
#include <Core/Content/ContentDatabase.h>

#include <Core/Serialization/FlatBuffers/Content_generated.h>

namespace Editor
{
	void ContentSerializer::serializeMaterial(Core::Material* value)
	{
		Core::String path = Core::ContentDatabase::singleton()->getPath(value->getUuid());
		serializeMaterial(value, path);
	}

	void ContentSerializer::serializeMaterial(Core::Material* value, Core::String path)
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

		std::ofstream file(path.std_str(), std::ios::binary);
		file.write(reinterpret_cast<const char*>(buf), size);
		file.close();
	}
} // namespace Editor