#include "Mesh.h"

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Renderer/VertexBuffer.h"
#include "../Assets/Material.h"
#include "../Assets/Texture.h"

#include "../Renderer/Renderer.h"

namespace Core
{
	/* SubMesh */

	SubMesh::SubMesh(const VertexBuffer* vertexBuffer)
	{
		_vertexBuffer = vertexBuffer;
	}

	SubMesh::~SubMesh()
	{
		if (_vertexBuffer != nullptr)
		{
			Renderer::singleton()->deleteBuffer(_vertexBuffer);
			_vertexBuffer = nullptr;
		}

		_material = nullptr;
	}

	/* Mesh */

	Mesh::Mesh(SubMesh** subMeshes, int count)
	{
		_subMeshes = subMeshes;
		_count = count;
	}

	Mesh::~Mesh()
	{
		if (isLoaded())
			unload();
	}

	const void Mesh::unload()
	{
		if (_subMeshes != nullptr)
		{
			for (int i = 0; i < _count; ++i)
				delete _subMeshes[i];

			delete[] _subMeshes;
		}

		_subMeshes = nullptr;
		_count = 0;

		Asset::unload();
	}

	Mesh* Mesh::loadFromFile(const char* fileName)
	{
		Assimp::Importer* importer = new Assimp::Importer();
		importer->SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.0f);
		importer->SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 60.0f);
		importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		unsigned int importFlags =
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_GlobalScale |
			aiProcess_CalcTangentSpace |
			aiProcess_PopulateArmatureData;

		importFlags |= aiProcess_JoinIdenticalVertices;
		importFlags |= aiProcess_GenSmoothNormals | aiProcess_ForceGenNormals;

		aiScene* _scene = const_cast<aiScene*>(importer->ReadFile(fileName, importFlags));

		SubMesh** _subMeshes = new SubMesh*[_scene->mNumMeshes];
		Mesh* _mesh = new Mesh(_subMeshes, _scene->mNumMeshes);

		_mesh->aab = AxisAlignedBox();

		for (int i = 0; i < _scene->mNumMeshes; ++i)
		{
			const aiMesh* mesh = _scene->mMeshes[i];
			std::vector<Vertex> verts;
			std::vector<UInt32> inds;

			for (int j = 0; j < mesh->mNumVertices; ++j)
			{
				Vertex vtx {};

				vtx.position[0] = mesh->mVertices[j].x;
				vtx.position[1] = mesh->mVertices[j].y;
				vtx.position[2] = mesh->mVertices[j].z;

				vtx.uv[0] = mesh->mTextureCoords[0][j].x;
				vtx.uv[1] = mesh->mTextureCoords[0][j].y;

				vtx.color[0] = mesh->mColors[0][j].r;
				vtx.color[1] = mesh->mColors[0][j].g;
				vtx.color[2] = mesh->mColors[0][j].b;
				vtx.color[3] = mesh->mColors[0][j].a;

				_mesh->aab.merge(vtx.getPosition());

				verts.push_back(vtx);
			}

			for (int j = 0; j < mesh->mNumFaces; ++j)
			{
				aiFace face = mesh->mFaces[j];
				inds.push_back(face.mIndices[0]);
				inds.push_back(face.mIndices[1]);
				inds.push_back(face.mIndices[2]);
			}

			const VertexBuffer* _vertexBuffer = Renderer::singleton()->createBuffer(&verts[0], verts.size(), &inds[0], inds.size());
			Material* material = new Material();
			Texture* texture = Texture::loadFromFile("D:/Dev/C++/core-engine/x64/Release/Test Project/diffuse.jpg", TextureFormat::BC7);
			material->setTexture(texture);

			SubMesh* subMesh = new SubMesh(_vertexBuffer);
			subMesh->setMaterial(material);

			_subMeshes[i] = subMesh;
		}

		delete importer;

		_mesh->load();

		return _mesh;
	}
}