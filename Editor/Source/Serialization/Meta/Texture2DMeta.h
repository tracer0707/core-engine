#pragma once

#include <Core/Shared/List.h>
#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>

#include <filesystem>

namespace Editor
{
	namespace Serialization
	{
		class Texture2DMeta
		{
			public:
				enum class Usage
				{
					Uncompressed,
					Color,
					Normal
				};

				enum class Quality
				{
					Low,
					Normal,
					High,
					Ultra
				};

				enum class Size
				{
					Quarter,
					Half,
					Full
				};

				void save(fs::path path) const;
				bool load(fs::path path);

				Usage usage = Usage::Color;
				Quality quality = Quality::High;
				Size size = Size::Full;
		};
	} // namespace Serialization
} // namespace Editor