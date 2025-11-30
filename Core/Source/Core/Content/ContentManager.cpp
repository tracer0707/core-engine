#include "ContentManager.h"

#include <fstream>

#include "../Renderer/Renderer.h"
#include "../Renderer/VertexBuffer.h"

#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderTexture.h"

#include "../Serialization/FlatBuffers/TextureSerializer_generated.h"

namespace Core
{
    ContentManager::ContentManager(Renderer* renderer)
    {
        _renderer = renderer;

        _defaultShader = loadShaderFromString("#version 400\n"
                                              "layout (location = 0) in vec3 position;"
                                              "layout (location = 1) in vec2 uv0;"
                                              "layout (location = 2) in vec4 color0;"
                                              "out vec2 f_uv0;"
                                              "out vec4 f_color0;"
                                              "uniform mat4 u_viewMtx;"
                                              "uniform mat4 u_projMtx;"
                                              "uniform mat4 u_modelMtx;"
                                              "void main() {"
                                              "  f_uv0 = uv0;"
                                              "  f_color0 = color0;"
                                              "  gl_Position = u_projMtx * u_viewMtx * u_modelMtx * vec4(position, 1.0);"
                                              "}",
                                              "#version 400\n"
                                              "in vec2 f_uv0;"
                                              "in vec4 f_color0;"
                                              "out vec4 frag_colour;"
                                              "uniform sampler2D u_diffuseTex;"
                                              "void main() {"
                                              "  vec4 out_color = texture2D(u_diffuseTex, f_uv0);"
                                              "  frag_colour = vec4(f_color0.rgb, 1.0);"
                                              "}");

        _defaultMaterial = createMaterial();
    }

    ContentManager::~ContentManager()
    {
        for (auto it : _materials)
            delete it;
        for (auto it : _shaders)
            delete it;
        for (auto it : _textures)
            delete it;
        for (auto it : _meshes)
            delete it;
        for (auto it : _renderTextures)
            delete it;

        _materials.clear();
        _shaders.clear();
        _textures.clear();
        _meshes.clear();
        _renderTextures.clear();

        _defaultMaterial = nullptr;
        _defaultShader = nullptr;
        _renderer = nullptr;
    }

    /* MATERIAL */

    Material* ContentManager::createMaterial()
    {
        Material* value = new Material(_renderer);
        value->setShader(_defaultShader);
        _materials.add(value);
        return value;
    }

    void ContentManager::destroyMaterial(Material* value)
    {
        _materials.remove(value);
        delete value;
    }

    /* TEXTURE */

    Texture* ContentManager::loadTextureFromFile(String fileName, TextureFormat fmt)
    {
        std::ifstream file(fileName.std_str(), std::ios::binary | std::ios::ate);
        size_t fileSize = file.tellg();
        file.seekg(0);

        std::vector<uint8_t> data(fileSize);
        file.read(reinterpret_cast<char*>(data.data()), fileSize);

        auto textureSerialized = GetTextureSerializer(data.data());

        unsigned char* dataRaw = const_cast<unsigned char*>(textureSerialized->data()->data());

        return new Texture(_renderer, textureSerialized->width(), textureSerialized->height(), dataRaw, textureSerialized->size(),
                           static_cast<TextureFormat>(textureSerialized->format()));
    }

    Texture* ContentManager::loadTextureFromBytes(unsigned char* data, int w, int h, int size, TextureFormat fmt)
    {
        return new Texture(_renderer, w, h, data, size, fmt);
    }

    void ContentManager::destroyTexture(Texture* value)
    {
        _textures.remove(value);
        delete value;
    }

    /* MESH */

    Mesh* ContentManager::createMesh(int subMeshesCount)
    {
        SubMesh** _subMeshes = new SubMesh*[subMeshesCount];
        Mesh* _mesh = new Mesh(_subMeshes, subMeshesCount);

        for (int i = 0; i < subMeshesCount; ++i)
        {
            _subMeshes[i] = new SubMesh(_renderer);
        }

        _meshes.add(_mesh);
        return _mesh;
    }

    Mesh* ContentManager::loadMeshFromFile(String fileName)
    {
        return nullptr;
    }

    void ContentManager::destroyMesh(Mesh* value)
    {
        _meshes.remove(value);
        delete value;
    }

    /* SHADER */

    Shader* ContentManager::loadShaderFromString(String vertexSrc, String fragmentSrc)
    {
        Shader* shader = new Shader(_renderer, vertexSrc, fragmentSrc);
        _shaders.add(shader);
        return shader;
    }

    void ContentManager::destroyShader(Shader* value)
    {
        _shaders.remove(value);
        delete value;
    }

    /* RENDERTEXTURE */

    RenderTexture* ContentManager::createRenderTexture(unsigned int width, unsigned int height)
    {
        RenderTexture* renderTexture = new RenderTexture(_renderer, width, height);
        _renderTextures.add(renderTexture);
        return renderTexture;
    }

    void ContentManager::destroyRenderTexture(RenderTexture* value)
    {
        _renderTextures.remove(value);
        delete value;
    }
} // namespace Core