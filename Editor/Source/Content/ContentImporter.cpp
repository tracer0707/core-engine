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
#include <Core/Serialization/FlatBuffers/TextureSerializer_generated.h>
#include <Core/Serialization/FlatBuffers/MeshSerializer_generated.h>

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
		auto _texture = Core::CreateTextureSerializer(builder, _width, _height, static_cast<int>(format), _size, data);
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
		Assimp::Importer* importer = new Assimp::Importer();
		importer->SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.0f);
		importer->SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 60.0f);
		importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		unsigned int importFlags =
			aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_GlobalScale | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData;

		importFlags |= aiProcess_JoinIdenticalVertices;
		importFlags |= aiProcess_GenSmoothNormals | aiProcess_ForceGenNormals;

		const aiScene* _scene = importer->ReadFile(sourceFileName.std_str().c_str(), importFlags);

		Core::AxisAlignedBox aab = Core::AxisAlignedBox();

		for (int i = 0; i < _scene->mNumMeshes; ++i)
		{
			const aiMesh* mesh = _scene->mMeshes[i];
			
		}

		delete importer;
	}
} // namespace Editor