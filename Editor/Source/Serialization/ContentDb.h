#pragma once

#include <map>

#include <Core/Shared/List.h>
#include <Core/Shared/String.h>
#include <Core/Shared/Uuid.h>

namespace Editor
{
    namespace Serialization
    {
        class ContentDb
        {
          public:
            static void save(Core::String filename, std::map<Core::String, Core::Uuid> data);
            static std::map<Core::String, Core::Uuid> load(Core::String filename);
        };
    }
}