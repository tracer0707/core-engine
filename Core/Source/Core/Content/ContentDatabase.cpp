#include "ContentDatabase.h"

#include <filesystem>

#include "../Shared/Path.h"
#include "../System/Application.h"
#include "../Serialization/JsonSerialization.h"

namespace fs = std::filesystem;

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

	String ContentDatabase::getRelativePath(String absolutePath)
	{
		String contentPath = _app->getContentPath();
		return Path::relative(absolutePath, contentPath);
	}

	String ContentDatabase::getAbsolutePath(String relativePath)
	{
		String contentPath = _app->getContentPath();
		return Path::combine(relativePath, contentPath);
	}

	String ContentDatabase::getPath(Uuid uuid)
	{
		if (_uuidToPath.find(uuid) != _uuidToPath.end())
		{
			return getAbsolutePath(_uuidToPath[uuid]);
		}
		else
		{
			throw std::runtime_error("Resource not found");
		}
	}

	Uuid ContentDatabase::getUuid(String path)
	{
		String relativePath = getRelativePath(path);

		if (_pathToUuid.find(relativePath) != _pathToUuid.end())
		{
			return _pathToUuid[relativePath];
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
		String relativePath = getRelativePath(path);
		return _pathToUuid.find(relativePath) != _pathToUuid.end();
	}

	void ContentDatabase::setPath(Uuid uuid, String path)
	{
		String relativePath = getRelativePath(path);

		_uuidToPath[uuid] = relativePath;
		_pathToUuid[relativePath] = uuid;
	}

	void ContentDatabase::dump(Core::String path) const
	{
		nlohmann::serialize(_pathToUuid, path);
	}
} // namespace Core
