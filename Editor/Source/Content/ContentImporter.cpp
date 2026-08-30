#include "ContentImporter.h"

#include <vector>
#include <fstream>
#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <FreeImage.h>

#include <Core/System/Application.h>
#include <Core/Shared/Uuid.h>
#include <Core/Shared/Path.h>
#include <Core/Math/AxisAlignedBox.h>
#include <Core/Renderer/Color.h>
#include <Core/Renderer/VertexBuffer.h>
#include <Core/Content/Texture2D.h>
#include <Core/Content/Mesh.h>
#include <Core/Content/Material.h>
#include <Core/Content/ContentManager.h>

#include "../Main/EditorApp.h"
#include "../Utils/bc7compressor.h"
#include "../Utils/TextureUtils.h"

#include "flatbuffers/flatbuffers.h"
#include <Core/Serialization/FlatBuffers/Content_generated.h>

namespace fs = std::filesystem;

namespace Editor
{
	ContentImporter::ContentImporter(EditorApp* app)
	{
		_app = app;

		FreeImage_Initialise();
	}

	ContentImporter::~ContentImporter()
	{
		FreeImage_DeInitialise();

		_app = nullptr;
	}

	void ContentImporter::importTexture2D(Core::String sourceFileName, Core::String targetFileName, Core::TextureFormat format)
	{
		int _width = 0;
		int _height = 0;
		unsigned char* _data = nullptr;
		int _size = 0;

		FREE_IMAGE_FORMAT _fmt = FreeImage_GetFileType(sourceFileName.std_str().c_str());
		FIBITMAP* texture = FreeImage_Load(_fmt, sourceFileName.std_str().c_str());

		FIBITMAP* convert = TextureUtils::makeSquare(texture);
		FreeImage_Unload(texture);
		texture = convert;

		if (FreeImage_GetBPP(texture) != 32)
		{
			FIBITMAP* convert = FreeImage_ConvertTo32Bits(texture);
			FreeImage_Unload(texture);
			texture = convert;
		}

		_width = FreeImage_GetWidth(texture);
		_height = FreeImage_GetHeight(texture);

		if (format == Core::TextureFormat::RGBA8)
		{
			unsigned char* src = FreeImage_GetBits(texture);
			unsigned int bpp = FreeImage_GetBPP(texture) / 8;
			unsigned int pitch = FreeImage_GetPitch(texture);
			_size = _width * _height * bpp;
			unsigned char* dst = new unsigned char[_size];

			for (unsigned y = 0; y < _height; y++)
			{
				memcpy(dst + y * _width * bpp, src + y * pitch, _width * bpp);
			}

			_data = dst;
		}
		else if (format == Core::TextureFormat::BC7)
		{
			color_quad_u8_vec pixels;
			_size = (((_width + 3) & ~3) * ((_height + 3) & ~3) * 8) >> 3;
			unsigned char* newPixels = new unsigned char[_size];
			TextureUtils::copyPixels(pixels, texture, _width, _height);
			bc7compress(pixels, _width, _height, newPixels, _size, 1);
			pixels.clear();
			_data = newPixels;
		}
		else
		{
			throw std::runtime_error("Texture format is unsupported");
		}

		FreeImage_Unload(texture);

		if (_data == nullptr)
		{
			throw std::runtime_error("Texture loading error");
		}

		flatbuffers::FlatBufferBuilder builder;

		auto data = builder.CreateVector(_data, _size);
		auto _texture = Core::Serialization::CreateTexture(builder, _width, _height, static_cast<int>(format), _size, data);
		builder.Finish(_texture);

		uint8_t* buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		std::ofstream file(targetFileName.std_str(), std::ios::binary);
		file.write(reinterpret_cast<const char*>(buf), size);
		file.close();

		delete[] _data;
	}

	void ContentImporter::importMesh(Core::String sourceFileName, Core::String targetFileName)
	{
		Assimp::Importer importer;

		importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.0f);
		importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 60.0f);
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const unsigned int importFlags = aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_GlobalScale | aiProcess_CalcTangentSpace |
										 aiProcess_PopulateArmatureData | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals |
										 aiProcess_ForceGenNormals;

		const aiScene* scene = importer.ReadFile(sourceFileName.std_str().c_str(), importFlags);

		if (!scene)
		{
			return;
		}

		if (!scene->mRootNode) return;

		std::function<void(const aiNode*)> processNode;

		int meshIndex = 0;
		processNode = [&](const aiNode* node) {
			if (node->mNumMeshes > 0)
			{
				std::vector<Core::Vertex> vertices;
				std::vector<uint32_t> indices;

				std::vector<std::pair<uint32_t, uint32_t>> subMeshes;

				Core::AxisAlignedBox aabb;

				for (unsigned int nodeMeshIndex = 0; nodeMeshIndex < node->mNumMeshes; ++nodeMeshIndex)
				{
					const unsigned int aiMeshIndex = node->mMeshes[nodeMeshIndex];

					const aiMesh* aiMesh = scene->mMeshes[aiMeshIndex];

					if (!aiMesh || aiMesh->mNumVertices == 0 || !aiMesh->HasFaces())
					{
						continue;
					}

					const uint32_t vertexOffset = static_cast<uint32_t>(vertices.size());
					const uint32_t indexOffset = static_cast<uint32_t>(indices.size());

					vertices.resize(vertices.size() + aiMesh->mNumVertices);

					for (unsigned int vertexIndex = 0; vertexIndex < aiMesh->mNumVertices; ++vertexIndex)
					{
						Core::Vertex& vertex = vertices[vertexOffset + vertexIndex];
						const aiVector3D& position = aiMesh->mVertices[vertexIndex];

						vertex._position = glm::vec3(position.x, position.y, position.z);

						aabb.merge(vertex._position);

						if (aiMesh->HasNormals())
						{
							const aiVector3D& normal = aiMesh->mNormals[vertexIndex];
							vertex._normal = glm::vec3(normal.x, normal.y, normal.z);
						}
						else
						{
							vertex._normal = glm::vec3(0.0f, 0.0f, 0.0f);
						}

						if (aiMesh->HasTangentsAndBitangents())
						{
							const aiVector3D& tangent = aiMesh->mTangents[vertexIndex];
							const aiVector3D& bitangent = aiMesh->mBitangents[vertexIndex];

							vertex._tangent = glm::vec3(tangent.x, tangent.y, tangent.z);
							vertex._bitangent = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
						}
						else
						{
							vertex._tangent = glm::vec3(0.0f, 0.0f, 0.0f);
							vertex._bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
						}

						if (aiMesh->HasTextureCoords(0))
						{
							const aiVector3D& uv = aiMesh->mTextureCoords[0][vertexIndex];
							vertex._uv0 = glm::vec2(uv.x, uv.y);
						}
						else
						{
							vertex._uv0 = glm::vec2(0.0f, 0.0f);
						}

						if (aiMesh->HasTextureCoords(1))
						{
							const aiVector3D& uv = aiMesh->mTextureCoords[1][vertexIndex];
							vertex._uv1 = glm::vec2(uv.x, uv.y);
						}
						else
						{
							vertex._uv1 = glm::vec2(0.0f, 0.0f);
						}

						if (aiMesh->HasVertexColors(0))
						{
							const aiColor4D& color = aiMesh->mColors[0][vertexIndex];
							vertex._color = Core::Color(color.r, color.g, color.b, color.a);
						}
						else
						{
							vertex._color = Core::Color(1.0f, 1.0f, 1.0f, 1.0f);
						}

						vertex._blendWeights = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
						vertex._blendIndices = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
					}

					for (unsigned int faceIndex = 0; faceIndex < aiMesh->mNumFaces; ++faceIndex)
					{
						const aiFace& face = aiMesh->mFaces[faceIndex];

						if (face.mNumIndices != 3) continue;

						indices.push_back(vertexOffset + face.mIndices[0]);
						indices.push_back(vertexOffset + face.mIndices[1]);
						indices.push_back(vertexOffset + face.mIndices[2]);
					}

					const uint32_t indexCount = static_cast<uint32_t>(indices.size()) - indexOffset;

					if (indexCount > 0)
					{
						subMeshes.emplace_back(indexOffset, indexCount);
					}
				}

				if (!vertices.empty() && !indices.empty() && !subMeshes.empty())
				{
					flatbuffers::FlatBufferBuilder builder;

					std::vector<Core::Serialization::Vertex> fVertices;
					for (auto& v : vertices)
					{
						fVertices.push_back(Core::Serialization::Vertex(
							Core::Serialization::Vec3(v._position.x, v._position.y, v._position.z),
							Core::Serialization::Vec3(v._normal.x, v._normal.y, v._normal.z),
							Core::Serialization::Vec3(v._tangent.x, v._tangent.y, v._tangent.z),
							Core::Serialization::Vec3(v._bitangent.x, v._bitangent.y, v._bitangent.z), Core::Serialization::Vec2(v._uv0.x, v._uv0.y),
							Core::Serialization::Vec2(v._uv1.x, v._uv1.y), Core::Serialization::Vec4(v._color.r, v._color.g, v._color.b, v._color.a),
							Core::Serialization::Vec4(v._blendWeights.x, v._blendWeights.y, v._blendWeights.z, v._blendWeights.w),
							Core::Serialization::Vec4(v._blendIndices.x, v._blendIndices.y, v._blendIndices.z, v._blendIndices.w)));
					}

					auto verticesOffset = builder.CreateVectorOfStructs(fVertices);
					auto indicesOffset = builder.CreateVector(indices);

					std::vector<flatbuffers::Offset<Core::Serialization::SubMesh>> subMeshOffsets;

					subMeshOffsets.reserve(subMeshes.size());

					for (const auto& [indexOffset, indexCount] : subMeshes)
					{
						subMeshOffsets.push_back(Core::Serialization::CreateSubMesh(builder, indexOffset, indexCount));
					}

					auto subMeshesOffset = builder.CreateVector(subMeshOffsets);

					auto aabbMinOffset = Core::Serialization::Vec3(aabb.getMinimum().x, aabb.getMinimum().y, aabb.getMinimum().z);
					auto aabbMaxOffset = Core::Serialization::Vec3(aabb.getMaximum().x, aabb.getMaximum().y, aabb.getMaximum().z);
					auto aabbOffset = Core::Serialization::AABB(aabbMinOffset, aabbMaxOffset);

					auto meshOffset = Core::Serialization::CreateMesh(builder, verticesOffset, indicesOffset, subMeshesOffset, &aabbOffset);

					builder.Finish(meshOffset);

					std::filesystem::path fileName = targetFileName.std_str();
					Core::String outputFileName = fileName
													  .replace_filename(fileName.filename().stem().generic_string() + "_" +
																		std::to_string(meshIndex) + fileName.extension().generic_string())
													  .generic_string();

					std::ofstream file(outputFileName.std_str(), std::ios::binary);

					if (file.is_open())
					{
						file.write(reinterpret_cast<const char*>(builder.GetBufferPointer()), static_cast<std::streamsize>(builder.GetSize()));
						file.close();
					}
				}

				++meshIndex;
			}

			for (unsigned int childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
			{
				processNode(node->mChildren[childIndex]);
			}
		};

		processNode(scene->mRootNode);
	}
} // namespace Editor