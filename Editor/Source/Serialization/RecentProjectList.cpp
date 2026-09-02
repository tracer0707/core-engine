#include "RecentProjectList.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <Core/Classes/json.hpp>
#include <Core/Shared/Path.h>
#include <Core/Serialization/JsonSerialization.h>

namespace fs = std::filesystem;

namespace Editor
{
    namespace Serialization
    {
        Core::List<fs::path> RecentProjectList::_projectList;

        static fs::path filename = fs::current_path() / fs::path("projects.json");

        void RecentProjectList::save()
        {
            nlohmann::serialize(_projectList, filename);
        }

        void RecentProjectList::load()
        {
			try
			{
				nlohmann::deserialize(_projectList, filename);
			}
			catch(...) {}
        }
    } // namespace Serialization
} // namespace Editor