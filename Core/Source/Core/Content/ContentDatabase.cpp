#include "ContentDatabase.h"

#include <filesystem>

#include "../System/Application.h"
#include "../Shared/Path.h"
#include "../Serialization/JsonSerialization.h"

namespace Core
{
	ContentDatabase::ContentDatabase(Application* app)
	{
		_app = app;

		Core::String dbPath = Core::Path::combine(_app->getRootPath(), "ContentDatabase.json");

		if (std::filesystem::exists(dbPath.std_str()))
		{
			nlohmann::deserialize(_pathToUuid, dbPath);

			for (auto& p : _pathToUuid)
			{
				_uuidToPath[p.second] = p.first;
			}
		}
	}

	ContentDatabase::~ContentDatabase()
	{
		_app = nullptr;
	}

	Core::String ContentDatabase::getPath(Core::Uuid uuid)
	{
		if (_uuidToPath.find(uuid) != _uuidToPath.end())
		{
			return _uuidToPath[uuid];
		}
		else
		{
			throw std::runtime_error("Resource not found");
		}
	}

	Core::Uuid ContentDatabase::getUuid(Core::String path)
	{
		if (_pathToUuid.find(path) != _pathToUuid.end())
		{
			return _pathToUuid[path];
		}
		else
		{
			throw std::runtime_error("Resource not found");
		}
	}

	void ContentDatabase::setPath(Core::Uuid uuid, Core::String sourcePath)
	{
		_uuidToPath[uuid] = sourcePath;
	}
} // namespace Core
