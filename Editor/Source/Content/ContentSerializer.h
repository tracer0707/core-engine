#pragma once

#include <Core/Shared/String.h>

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
			static void serializeMaterial(Core::Material* value, Core::String path);

			static void serializeScene(Core::Scene* value);
			static void serializeScene(Core::Scene* value, Core::String path);
	};
} // namespace Editor