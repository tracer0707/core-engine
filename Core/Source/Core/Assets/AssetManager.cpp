#include "AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <FreeImage.h>

#include "../Renderer/Renderer.h"
#include "../Renderer/VertexBuffer.h"

#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderTexture.h"

namespace Core
{
    AssetManager::AssetManager(Renderer* renderer)
    {
        FreeImage_Initialise();

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

    AssetManager::~AssetManager()
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

        FreeImage_DeInitialise();
    }

    /* MATERIAL */

    Material* AssetManager::createMaterial()
    {
        Material* value = new Material(_renderer);
        value->setShader(_defaultShader);
        _materials.add(value);
        return value;
    }

    void AssetManager::destroyMaterial(Material* value)
    {
        _materials.remove(value);
        delete value;
    }

    /* TEXTURE */

    Texture* AssetManager::loadTextureFromFile(String fileName, TextureFormat fmt)
    {
        FREE_IMAGE_FORMAT _fmt = FreeImage_GetFileType(fileName.std_str().c_str());
        FIBITMAP* texture = FreeImage_Load(_fmt, fileName.std_str().c_str());

        if (FreeImage_GetBPP(texture) != 32)
        {
            FIBITMAP* convert = FreeImage_ConvertTo32Bits(texture);
            FreeImage_Unload(texture);
            texture = convert;
        }

        Texture* tex = new Texture(_renderer, texture, fmt);
        _textures.add(tex);
        return tex;
    }

    Texture* AssetManager::loadTextureFromBytes(unsigned char* data, int w, int h, int bpp, TextureFormat fmt)
    {
        FIBITMAP* texture = FreeImage_Allocate(w, h, bpp * 8, 8, 8, 8);
        for (std::int32_t i = 0; i < h; ++i)
        {
            BYTE* bits = FreeImage_GetScanLine(texture, h - 1 - i);
            for (std::int32_t j = 0; j < w; ++j)
            {
                bits[0] = data[(i * w + j) * 4 + 2];
                bits[1] = data[(i * w + j) * 4 + 1];
                bits[2] = data[(i * w + j) * 4 + 0];
                bits[3] = data[(i * w + j) * 4 + 3];
                bits += 4;
            }
        }

        FIBITMAP* conv = FreeImage_ConvertTo32Bits(texture);
        FreeImage_Unload(texture);
        texture = conv;

        Texture* tex = new Texture(_renderer, texture, fmt);
        _textures.add(tex);
        return tex;
    }

    void AssetManager::destroyTexture(Texture* value)
    {
        _textures.remove(value);
        delete value;
    }

    /* MESH */

    Mesh* AssetManager::createMesh(int subMeshesCount)
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

    Mesh* AssetManager::loadMeshFromFile(String fileName)
    {
        Assimp::Importer* importer = new Assimp::Importer();
        importer->SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.0f);
        importer->SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 60.0f);
        importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

        unsigned int importFlags =
            aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_GlobalScale | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData;

        importFlags |= aiProcess_JoinIdenticalVertices;
        importFlags |= aiProcess_GenSmoothNormals | aiProcess_ForceGenNormals;

        const aiScene* _scene = importer->ReadFile(fileName.std_str().c_str(), importFlags);

        Mesh* _mesh = createMesh(_scene->mNumMeshes);
        SubMesh** _subMeshes = _mesh->getSubMeshes();

        AxisAlignedBox aab = AxisAlignedBox();

        for (int i = 0; i < _scene->mNumMeshes; ++i)
        {
            const aiMesh* mesh = _scene->mMeshes[i];
            std::vector<Vertex> verts;
            std::vector<unsigned int> inds;

            for (int j = 0; j < mesh->mNumVertices; ++j)
            {
                Vertex vtx{};

                vtx.position[0] = mesh->mVertices[j].x;
                vtx.position[1] = mesh->mVertices[j].y;
                vtx.position[2] = mesh->mVertices[j].z;

                vtx.uv[0] = mesh->mTextureCoords[0][j].x;
                vtx.uv[1] = mesh->mTextureCoords[0][j].y;

                vtx.color[0] = mesh->mColors[0][j].r;
                vtx.color[1] = mesh->mColors[0][j].g;
                vtx.color[2] = mesh->mColors[0][j].b;
                vtx.color[3] = mesh->mColors[0][j].a;

                aab.merge(vtx.getPosition());

                verts.push_back(vtx);
            }

            for (int j = 0; j < mesh->mNumFaces; ++j)
            {
                aiFace face = mesh->mFaces[j];
                inds.push_back(face.mIndices[0]);
                inds.push_back(face.mIndices[1]);
                inds.push_back(face.mIndices[2]);
            }

            Material* material = createMaterial();
            Texture* texture = loadTextureFromFile("D:/Dev/C++/core-engine/x64/Release/Test Project/diffuse.jpg", TextureFormat::BC7);
            material->setTexture(texture);

            _subMeshes[i]->setMaterial(material);
            _subMeshes[i]->updateVertexBuffer(&verts[0], verts.size(), &inds[0], inds.size());
        }

        _mesh->setBoundingBox(aab);

        delete importer;
        return _mesh;
    }

    void AssetManager::destroyMesh(Mesh* value)
    {
        _meshes.remove(value);
        delete value;
    }

    /* SHADER */

    Shader* AssetManager::loadShaderFromString(String vertexSrc, String fragmentSrc)
    {
        Shader* shader = new Shader(_renderer, vertexSrc, fragmentSrc);
        _shaders.add(shader);
        return shader;
    }

    void AssetManager::destroyShader(Shader* value)
    {
        _shaders.remove(value);
        delete value;
    }

    /* RENDERTEXTURE */

    RenderTexture* AssetManager::createRenderTexture(unsigned int width, unsigned int height)
    {
        RenderTexture* renderTexture = new RenderTexture(_renderer, width, height);
        _renderTextures.add(renderTexture);
        return renderTexture;
    }

    void AssetManager::destroyRenderTexture(RenderTexture* value)
    {
        _renderTextures.remove(value);
        delete value;
    }
} // namespace Core