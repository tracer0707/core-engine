#include "TextureManager.h"

#include <cstring>

#include <FreeImage.h>

#include <Core/Shared/String.h>

#ifdef _WIN32
#include <Core/Shared/Path.h>
#endif

#include "Texture.h"
#include "../Utils/TextureUtils.h"

namespace Editor
{
	TextureManager::TextureManager(Core::Renderer* renderer)
	{
		_renderer = renderer;
	}

	TextureManager::~TextureManager()
	{
		for (auto& pair : _textureCache)
		{
			delete pair.second;
		}

		_textureCache.clear();
		_iconCache.clear();
	}

	Texture* TextureManager::loadFromFile(const fs::path& fileName)
	{
		if (fileName.empty() || !fs::exists(fileName)) return nullptr;

		if (_textureCache.find(fileName) != _textureCache.end())
		{
			return _textureCache[fileName];
		}

		FREE_IMAGE_FORMAT _fmt;
		FIBITMAP* texture;
#ifdef _WIN32
		const std::wstring wideFileName = fileName.wstring();
		_fmt = FreeImage_GetFileTypeU(wideFileName.c_str());
		texture = FreeImage_LoadU(_fmt, wideFileName.c_str());
#else
		_fmt = FreeImage_GetFileType(fileName.string().c_str());
		texture = FreeImage_Load(_fmt, fileName.string().c_str());
#endif
		if (texture == nullptr) return nullptr;

		FIBITMAP* convert = TextureUtils::makeSquare(texture);
		FreeImage_Unload(texture);
		texture = convert;

		if (FreeImage_GetBPP(texture) != 32)
		{
			FIBITMAP* convert = FreeImage_ConvertTo32Bits(texture);
			FreeImage_Unload(texture);
			texture = convert;
		}

		unsigned int w = FreeImage_GetWidth(texture);
		unsigned int h = FreeImage_GetHeight(texture);

		unsigned char* src = FreeImage_GetBits(texture);
		unsigned int bpp = FreeImage_GetBPP(texture) / 8;
		unsigned int pitch = FreeImage_GetPitch(texture);
		unsigned int size = w * h * bpp;
		unsigned char* dst = new unsigned char[size];

		for (unsigned y = 0; y < h; y++)
		{
			memcpy(dst + y * w * bpp, src + y * pitch, w * bpp);
		}

		Texture* result = new Texture(_renderer, dst, w, h);
		_textureCache[fileName] = result;

		delete[] dst;

		return result;
	}

	Texture* TextureManager::getIcon(EditorIcon icon)
	{
		if (_iconCache.find(icon) != _iconCache.end())
		{
			return _iconCache[icon];
		}

		return nullptr;
	}

	void TextureManager::loadIcons(const fs::path& rootPath)
	{
		_iconCache[EditorIcon::Save] = loadFromFile(rootPath / fs::path("save.png"));
		_iconCache[EditorIcon::Undo] = loadFromFile(rootPath / fs::path("undo.png"));
		_iconCache[EditorIcon::Redo] = loadFromFile(rootPath / fs::path("redo.png"));
		_iconCache[EditorIcon::Select] = loadFromFile(rootPath / fs::path("select.png"));
		_iconCache[EditorIcon::Translate] = loadFromFile(rootPath / fs::path("translate.png"));
		_iconCache[EditorIcon::Rotate] = loadFromFile(rootPath / fs::path("rotate.png"));
		_iconCache[EditorIcon::Scale] = loadFromFile(rootPath / fs::path("scale.png"));
		_iconCache[EditorIcon::Bounds] = loadFromFile(rootPath / fs::path("bounds.png"));
		_iconCache[EditorIcon::LocalSpace] = loadFromFile(rootPath / fs::path("local_space.png"));
		_iconCache[EditorIcon::WorldSpace] = loadFromFile(rootPath / fs::path("world_space.png"));
		_iconCache[EditorIcon::Object] = loadFromFile(rootPath / fs::path("object.png"));
		_iconCache[EditorIcon::PointLight] = loadFromFile(rootPath / fs::path("point_light.png"));
		_iconCache[EditorIcon::Camera] = loadFromFile(rootPath / fs::path("camera.png"));
		_iconCache[EditorIcon::Add] = loadFromFile(rootPath / fs::path("add.png"));
		_iconCache[EditorIcon::ChevronDown] = loadFromFile(rootPath / fs::path("chevron_down.png"));
		_iconCache[EditorIcon::Folder] = loadFromFile(rootPath / fs::path("folder.png"));
		_iconCache[EditorIcon::Font] = loadFromFile(rootPath / fs::path("font.png"));
		_iconCache[EditorIcon::Texture] = loadFromFile(rootPath / fs::path("texture.png"));
		_iconCache[EditorIcon::Mesh] = loadFromFile(rootPath / fs::path("mesh.png"));
		_iconCache[EditorIcon::Material] = loadFromFile(rootPath / fs::path("material.png"));
		_iconCache[EditorIcon::Scene] = loadFromFile(rootPath / fs::path("scene.png"));
		_iconCache[EditorIcon::Script] = loadFromFile(rootPath / fs::path("script.png"));
		_iconCache[EditorIcon::File] = loadFromFile(rootPath / fs::path("file.png"));

	}
} // namespace Editor