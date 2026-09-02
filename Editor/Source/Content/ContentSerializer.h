#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>

namespace Core
{
	class Material;
	class Scene;
}

namespace Editor
{
	class ContentSerializer
	{
		public:
			static void serializeMaterial(Core::Material* value);
			static void serializeMaterial(Core::Material* value, const fs::path& path);

			static void serializeScene(Core::Scene* value);
			static void serializeScene(Core::Scene* value, const fs::path& path);
	};
} // namespace Editor