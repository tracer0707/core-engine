#include "ContentSerializer.h"

#include <fstream>

#include <Core/Serialization/FlatBuffers/MaterialSerializer_generated.h>
#include <Core/Content/Material.h>
#include <Core/Content/Texture.h>
#include <Core/Content/ContentDatabase.h>

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

		auto texUuid = builder.CreateString(value->getTexture() != nullptr ? value->getTexture()->getUuid().toString() : "");
		auto material = Core::CreateMaterialSerializer(builder, texUuid);
		builder.Finish(material);

		uint8_t* buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		std::ofstream file(path.std_str(), std::ios::binary);
		file.write(reinterpret_cast<const char*>(buf), size);
		file.close();
	}
} // namespace Editor