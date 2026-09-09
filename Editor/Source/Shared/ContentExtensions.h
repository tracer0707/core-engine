#pragma once

#include <vector>
#include <Core/Shared/String.h>

namespace Editor
{
	class ContentExtensions
	{
		public:
			static std::vector<Core::String> getSupportedTextureExtensions();
			static std::vector<Core::String> getSupportedMeshExtensions();
	};

	std::vector<Core::String> ContentExtensions::getSupportedTextureExtensions()
	{
		return {".jpg",	 ".jpeg", ".jpe", ".jif", ".jfif", ".jfi",	".png",	 ".gif", ".bmp",  ".dib", ".tiff", ".tif", ".webp", ".heic",
				".heif", ".avif", ".cr2", ".cr3", ".crw",  ".nef",	".nrw",	 ".arw", ".srf",  ".sr2", ".raf",  ".orf", ".rw2",	".raw",
				".dng",	 ".psd",  ".psb", ".ai",  ".eps",  ".epsf", ".epsi", ".svg", ".svgz", ".tga", ".dds",  ".exr", ".hdr",	".ico",
				".icns", ".jp2",  ".j2k", ".jxr", ".jxl",  ".apng", ".pcx",	 ".xbm", ".xpm",  ".pnm", ".ppm",  ".pgm", ".pbm"};
	}

	std::vector<Core::String> ContentExtensions::getSupportedMeshExtensions()
	{
		return {".fbx", ".3ds", ".obj", ".glb"};
	}
} // namespace Editor