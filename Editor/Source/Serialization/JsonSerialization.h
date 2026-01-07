#pragma once

#include <Core/Classes/json.hpp>
#include <Core/Serialization/JsonSerialization.h>
#include <Core/Shared/String.h>
#include <Core/Shared/List.h>
#include <Core/Shared/Uuid.h>
#include <Core/Content/Material.h>
#include <Core/Content/Shader.h>
#include <Core/Content/Texture.h>

#include "../System/ContentDatabase.h"
#include "../System/ContentLoader.h"

namespace nlohmann
{
	template <> struct adl_serializer<Core::Material*>
	{
		static void to_json(json& j, const Core::Material* mat)
		{
			if (mat->getTexture() != nullptr)
			{
				j["texture"] = mat->getTexture()->getUuid();
			}

			if (mat->getShader() != nullptr)
			{
				j["shader"] = mat->getShader()->getUuid();
			}
		}

		static void from_json(const json& j, Core::Material* mat) {}
	};
} // namespace nlohmann