#pragma once

#include <Core/Shared/String.h>

namespace Core
{
	class Texture;
}

namespace Editor
{
	class ContentLoader;

	Core::Texture* loadEditorIcon(ContentLoader* loader, Core::String path);
} // namespace Editor