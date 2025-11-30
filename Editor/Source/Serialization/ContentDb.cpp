#include "ContentDb.h"

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
        void ContentDb::save(Core::String filename, std::map<Core::String, Core::Uuid> data)
		{
            nlohmann::json j = nlohmann::json{{"ContentDatabase", data}};

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

        std::map<Core::String, Core::Uuid> ContentDb::load(Core::String filename)
		{
            std::ifstream file(filename.std_str());
            if (file.is_open())
            {
                nlohmann::json j;
                file >> j;
                file.close();
                return j["ContentDatabase"].get<std::map<Core::String, Core::Uuid>>();
            }
            else
            {
                std::cout << "Project list file not found. Creating one..." << std::endl;
            }
		}
	}
}