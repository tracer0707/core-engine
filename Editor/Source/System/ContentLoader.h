#pragma once

#include <Core/Shared/String.h>
#include <Core/Renderer/TextureFormat.h>

namespace Core
{
    class Material;
    class Texture;
    class Mesh;
    class Shader;
    class RenderTexture;
    class ContentManager;
}

namespace Editor
{
    class Editor;
    class ContentDatabase;

    enum class LoadMethod
    {
        Cached,
        Direct
    };

    class ContentLoader
    {
      private:
        Editor* _app = nullptr;
        ContentDatabase* _contentDb = nullptr;
        Core::ContentManager* _contentMgr = nullptr;

        Core::String resolveTargetPath(Core::String sourcePath);

        void loadTexture(Core::String fileName, Core::TextureFormat format, unsigned char** data, int& size, int& w, int& h);

      public:
        ContentLoader(Editor* app, ContentDatabase* contentDb, Core::ContentManager* contentMgr);
        ~ContentLoader();

        ContentDatabase* getContentDatabase() { return _contentDb; }

        Core::ContentManager* getContentManager() { return _contentMgr; }
        Core::Texture* loadTextureFromFile(Core::String fileName, Core::TextureFormat format, LoadMethod loadMethod);
        Core::Texture* loadTextureFromBytes(unsigned char* data, int w, int h, int bpp, Core::TextureFormat format);
        Core::Mesh* loadMeshFromFile(Core::String fileName);
        Core::Shader* loadShaderFromString(Core::String vertexSrc, Core::String fragmentSrc);
    };
}