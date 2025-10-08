#include "RecentProjectList.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <Core/Classes/json.hpp>
#include <Core/Shared/Path.h>
#include <Core/Serialization/JsonSerialization.h>

namespace Editor
{
    Core::List<Core::String> RecentProjectList::_projectList;

    static Core::String filename = Core::Path::combine(std::filesystem::current_path().generic_string(), "projects.json");

    void RecentProjectList::save()
    {
        nlohmann::json j = nlohmann::json{ { "recent_projects", _projectList } };

        std::ofstream file(filename.std_str());
        if (file.is_open()) {
            file << j.dump(4);
            file.close();
        }
        else {
            throw new std::runtime_error("Error opening file for write");
        }
    }

	void RecentProjectList::load()
	{
        std::ifstream file(filename.std_str());
        if (file.is_open()) {
            nlohmann::json j;
            file >> j;
            file.close();
            _projectList = j["recent_projects"].get<Core::List<Core::String>>();
        }
        else {
            throw new std::runtime_error("Error opening file for read");
        }
	}
}