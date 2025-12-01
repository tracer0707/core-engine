#include "ThumbCacheManager.h"

#include <filesystem>
#include <vector>

#include <FreeImage.h>
#include <Core/Shared/Path.h>

#include "../Main/Editor.h"
#include "../System/ContentLoader.h"
#include "../Utils/TextureUtils.h"

namespace fs = std::filesystem;

namespace Editor
{
    static std::vector<Core::String> _supportedFormats = {".jpg", ".png", ".bmp", ".tga", ".tif", ".gif", ".psd"};

    ThumbCacheManager::ThumbCacheManager(Editor* app, ContentLoader* contentLoader)
    {
        _app = app;
        _contentLoader = contentLoader;
    }

    ThumbCacheManager::~ThumbCacheManager()
    {
        _app = nullptr;
    }

    Core::Texture* ThumbCacheManager::getOrCreate(Core::String filePath)
    {
        auto parentPath = fs::path(filePath.std_str()).parent_path().lexically_relative(_app->getContentPath().std_str());
        auto relativePath = fs::path(filePath.std_str()).lexically_relative(_app->getContentPath().std_str());
        auto thumbPath = Core::Path::combine(_app->getCacheThumbPath(), relativePath.generic_string());
        auto thumbDir = Core::Path::combine(_app->getCacheThumbPath(), parentPath.generic_string());

        if (fs::exists(thumbPath.std_str()))
        {
            return _contentLoader->loadTextureFromFile(thumbPath, Core::TextureFormat::RGBA8, LoadMethod::Direct);
        }

        if (!relativePath.has_extension()) return nullptr;
        
        auto ext = relativePath.extension().generic_string();

        if (std::find(_supportedFormats.begin(), _supportedFormats.end(), ext) != _supportedFormats.end())
        {
            FREE_IMAGE_FORMAT _fmt = FreeImage_GetFileType(filePath.std_str().c_str());
            FIBITMAP* texture = FreeImage_Load(_fmt, filePath.std_str().c_str());

            FIBITMAP* convert = TextureUtils::rescale(texture, 128, 128);
            FreeImage_Unload(texture);
            texture = convert;

            FreeImage_Save(_fmt, texture, thumbPath.std_str().c_str());

            if (!fs::exists(thumbDir.std_str()))
            {
                fs::create_directories(thumbDir.std_str());
            }

            return _contentLoader->loadTextureFromFile(thumbPath, Core::TextureFormat::RGBA8, LoadMethod::Direct);
        }

        return nullptr;
    }

} // namespace Editor