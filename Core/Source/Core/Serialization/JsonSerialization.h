#pragma once

#include <stdexcept>
#include <iostream>
#include <fstream>

#include "../Classes/json.hpp"
#include "../Shared/String.h"
#include "../Shared/List.h"
#include "../Shared/Uuid.h"

namespace nlohmann
{
	template <>
	struct adl_serializer<Core::String>
	{
			static void to_json(json& j, const Core::String& p) { j = p.std_str(); }

			static void from_json(const json& j, Core::String& p)
			{
				if (!j.is_string())
				{
					throw std::runtime_error("type must be string, but is " + std::string(j.type_name()));
				}

				p = j.get<std::string>();
			}
	};

	template <>
	struct adl_serializer<Core::Uuid>
	{
			static void to_json(json& j, const Core::Uuid& p) { j = p.toString(); }

			static void from_json(const json& j, Core::Uuid& p)
			{
				if (!j.is_string())
				{
					throw std::runtime_error("type must be string, but is " + std::string(j.type_name()));
				}

				std::string str = j.get<std::string>();
				p = Core::Uuid::fromString(str);
			}
	};

	template <typename T>
	struct adl_serializer<Core::List<T>>
	{
			static void to_json(json& j, const Core::List<T>& list)
			{
				j = json::array();
				for (const auto& item : list)
				{
					j.push_back(item);
				}
			}

			static void from_json(const json& j, Core::List<T>& list)
			{
				if (!j.is_array())
				{
					throw std::runtime_error("type must be array, but is " + std::string(j.type_name()));
				}

				list.clear();
				for (const auto& item : j)
				{
					list.add(item.get<T>());
				}
			}
	};

	template <typename T>
	void serialize(const T& obj, Core::String filename)
	{
		nlohmann::json j = nlohmann::json{{"data", obj}};

		std::ofstream file(filename.std_str());
		if (file.is_open())
		{
			file << j.dump(4);
			file.close();
		}
		else
		{
			throw std::runtime_error("Error opening file for write");
		}
	}

	template <typename T>
	void deserialize(T& obj, Core::String filename)
	{
		std::ifstream file(filename.std_str());
		if (file.is_open())
		{
			nlohmann::json j;
			file >> j;
			file.close();
			obj = j["data"].get<T>();
		}
		else
		{
			throw std::runtime_error("Error opening file for read");
		}
	}
} // namespace nlohmann