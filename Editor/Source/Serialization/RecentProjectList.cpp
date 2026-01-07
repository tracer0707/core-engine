#include "RecentProjectList.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <Core/Classes/json.hpp>
#include <Core/Shared/Path.h>
#include <Core/Serialization/JsonSerialization.h>

namespace Editor
{
    namespace Serialization
    {
        Core::List<Core::String> RecentProjectList::_projectList;

        static Core::String filename = Core::Path::combine(std::filesystem::current_path().generic_string(), "projects.json");

        void RecentProjectList::save()
        {
			nlohmann::serialize(_projectList, filename.std_str());
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