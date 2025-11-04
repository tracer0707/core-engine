#pragma once

#include <stdexcept>

#include "../Classes/json.hpp"
#include "../Shared/String.h"
#include "../Shared/List.h"

namespace nlohmann
{
    template <> struct adl_serializer<Core::String>
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

    template <typename T> struct adl_serializer<Core::List<T>>
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
} // namespace nlohmann