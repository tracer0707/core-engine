#include "ContentDatabase.h"

#include <filesystem>

#include "../Shared/Path.h"
#include "../System/Application.h"
#include "../Serialization/JsonSerialization.h"

namespace Core
{
	ContentDatabase::ContentDatabase(Application* app)
	{
		_app = app;

		String dbPath = Path::combine(_app->getRootPath(), "ContentDatabase.json");

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

	String ContentDatabase::getPath(Uuid uuid)
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

	Uuid ContentDatabase::getUuid(String path)
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

	bool ContentDatabase::hasPath(Uuid uuid)
	{
		return _uuidToPath.find(uuid) != _uuidToPath.end();
	}

	bool ContentDatabase::hasUuid(String path)
	{
		return _pathToUuid.find(path) != _pathToUuid.end();
	}

	void ContentDatabase::setPath(Uuid uuid, String path)
	{
		_uuidToPath[uuid] = path;
		_pathToUuid[path] = uuid;
	}
} // namespace Core
