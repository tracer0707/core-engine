#include "ContentDatabase.h"

#include "../Shared/Path.h"
#include "../System/Application.h"
#include "../Serialization/JsonSerialization.h"

namespace fs = std::filesystem;

namespace Core
{
	ContentDatabase ContentDatabase::_singleton;

	ContentDatabase::ContentDatabase() {}

	ContentDatabase::~ContentDatabase()
	{
		_app = nullptr;
	}

	fs::path ContentDatabase::getRelativePath(const fs::path& absolutePath)
	{
		return fs::relative(absolutePath, _app->getContentPath());
	}

	fs::path ContentDatabase::getAbsolutePath(const fs::path& relativePath)
	{
		return _app->getContentPath() / relativePath;
	}

	fs::path ContentDatabase::getPath(Uuid uuid)
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

	Uuid ContentDatabase::getUuid(const fs::path& path)
	{
		fs::path relativePath = getRelativePath(path);

		if (_pathToUuid.find(relativePath) != _pathToUuid.end())
		{
			return _pathToUuid[relativePath];
		}
		else
		{
			Uuid uuid = Uuid::create();
			_pathToUuid[relativePath] = uuid;
			_uuidToPath[uuid] = relativePath;

			return uuid;
		}
	}

	bool ContentDatabase::hasPath(Uuid uuid)
	{
		return _uuidToPath.find(uuid) != _uuidToPath.end();
	}

	bool ContentDatabase::hasUuid(const fs::path& path)
	{
		fs::path relativePath = getRelativePath(path);
		return _pathToUuid.find(relativePath) != _pathToUuid.end();
	}

	void ContentDatabase::setPath(Uuid uuid, const fs::path& path)
	{
		fs::path relativePath = getRelativePath(path);

		_uuidToPath[uuid] = relativePath;
		_pathToUuid[relativePath] = uuid;
	}

	void ContentDatabase::load()
	{
		if (fs::exists(_filePath))
		{
			std::map<String, Uuid> serializedPaths;
			nlohmann::deserialize(serializedPaths, _filePath);
			for (const auto& [path, uuid] : serializedPaths)
			{
				_pathToUuid[Path::fromUtf8(path)] = uuid;
			}

			for (auto& p : _pathToUuid)
			{
				_uuidToPath[p.second] = p.first;
			}
		}
	}

	void ContentDatabase::save() const
	{
		std::map<String, Uuid> serializedPaths;
		for (const auto& [path, uuid] : _pathToUuid)
		{
			serializedPaths[Path::toUtf8(path)] = uuid;
		}
		nlohmann::serialize(serializedPaths, _filePath);
	}
} // namespace Core
