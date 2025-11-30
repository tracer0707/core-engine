#include "ContentDatabase.h"

#include <filesystem>

#include <Core/Shared/Path.h>

#include "../../Main/Editor.h"
#include "../../Serialization/ContentDb.h"
#include "../Utils/FileSystemUtils.h"

namespace Editor
{
    ContentDatabase::ContentDatabase(Editor* app)
    {
        _app = app;

        Core::String dbPath = Core::Path::combine(_app->getRootPath(), "ContentDatabase.json");

        if (std::filesystem::exists(dbPath.std_str()))
        {
            _pathToUuid = Serialization::ContentDb::load(dbPath);

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

    void ContentDatabase::rebuild()
    {
        Core::String dbPath = Core::Path::combine(_app->getRootPath(), "ContentDatabase.json");

        if (std::filesystem::exists(dbPath.std_str()))
        {
            std::filesystem::remove(dbPath.std_str());
        }

        _pathToUuid.clear();
        _uuidToPath.clear();

        Core::List<std::filesystem::path> files;
        FileSystemUtils::enumerateFiles(_app->getContentPath().std_str(), files);

        for (auto& path : files)
        {
            Core::String pathStr = path.generic_string();
            Core::Uuid id = Core::Uuid::create();

            _uuidToPath[id] = pathStr;
            _pathToUuid[pathStr] = id;
        }

        Serialization::ContentDb::save(dbPath, _pathToUuid);
    }

    void ContentDatabase::update()
    {
    }

    Core::String ContentDatabase::getSourcePath(Core::Uuid uuid)
    {
        return _uuidToPath[uuid];
    }

    Core::String ContentDatabase::getTargetPath(Core::Uuid uuid)
    {
        Core::String path = Core::Path::combine(_app->getLibraryContentPath(), uuid.to_string().substr(0, 2));
        if (!std::filesystem::exists(path.std_str()))
        {
            std::filesystem::create_directories(path.std_str());
        }

        return Core::Path::combine(uuid.to_string().substr(0, 2), uuid.to_string() + ".bin");
    }

    Core::Uuid ContentDatabase::getUuid(Core::String sourcePath)
    {
        return _pathToUuid[sourcePath];
    }

    void ContentDatabase::setPath(Core::Uuid uuid, Core::String sourcePath)
    {
        _uuidToPath[uuid] = sourcePath;
    }
} // namespace Editor
