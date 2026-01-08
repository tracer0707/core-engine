#pragma once

#include <Core/Shared/String.h>

namespace Core
{
	class Material;
}

namespace Editor
{
	class ContentSerializer
	{
		public:
			static void serializeMaterial(Core::Material* value, Core::String path);
	};
} // namespace Editor