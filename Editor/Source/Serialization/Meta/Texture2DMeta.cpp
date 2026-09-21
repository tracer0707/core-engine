#include "Texture2DMeta.h"

#include <iostream>
#include <fstream>

#include <Core/Classes/json.hpp>
#include <Core/Shared/Path.h>
#include <Core/Serialization/JsonSerialization.h>

namespace fs = std::filesystem;

namespace nlohmann
{
	NLOHMANN_JSON_SERIALIZE_ENUM(Editor::Serialization::Texture2DMeta::Usage,
								 {{Editor::Serialization::Texture2DMeta::Usage::Color, "color"},
								  {Editor::Serialization::Texture2DMeta::Usage::Normal, "normal"},
								  {Editor::Serialization::Texture2DMeta::Usage::Uncompressed, "uncompressed"}})

	NLOHMANN_JSON_SERIALIZE_ENUM(Editor::Serialization::Texture2DMeta::Quality, {{Editor::Serialization::Texture2DMeta::Quality::Low, "low"},
																				 {Editor::Serialization::Texture2DMeta::Quality::Normal, "normal"},
																				 {Editor::Serialization::Texture2DMeta::Quality::High, "high"},
																				 {Editor::Serialization::Texture2DMeta::Quality::Ultra, "ultra"}})

	NLOHMANN_JSON_SERIALIZE_ENUM(Editor::Serialization::Texture2DMeta::Size, {{Editor::Serialization::Texture2DMeta::Size::Quarter, "quarter"},
																			  {Editor::Serialization::Texture2DMeta::Size::Half, "half"},
																			  {Editor::Serialization::Texture2DMeta::Size::Full, "full"}})

	void to_json(nlohmann::json& j, const Editor::Serialization::Texture2DMeta& meta)
	{
		j = nlohmann::json{{"usage", meta.usage},
						   {"quality", meta.quality},
						   {"size", meta.size}};
	}

	void from_json(const nlohmann::json& j, Editor::Serialization::Texture2DMeta& meta)
	{
		j.at("usage").get_to(meta.usage);
		j.at("quality").get_to(meta.quality);
		j.at("size").get_to(meta.size);
	}
}

namespace Editor
{
	namespace Serialization
	{
		void Texture2DMeta::save(fs::path path) const
		{
			nlohmann::serialize(*this, path);
		}

		bool Texture2DMeta::load(fs::path path)
		{
			try
			{
				nlohmann::deserialize(*this, path);
			}
			catch (...)
			{
				return false;
			}

			return true;
		}
	} // namespace Serialization
} // namespace Editor