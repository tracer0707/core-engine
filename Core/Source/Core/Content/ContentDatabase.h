#pragma once

#include <map>

#include "../Shared/Uuid.h"
#include "../Shared/String.h"

namespace Core
{
	class Application;

	class ContentDatabase
	{
		private:
			Application* _app = nullptr;

			std::map<Core::String, Core::Uuid> _pathToUuid;
			std::map<Core::Uuid, Core::String> _uuidToPath;

		public:
			ContentDatabase(Application* app);
			~ContentDatabase();

			Core::String getPath(Core::Uuid uuid);
			Core::Uuid getUuid(Core::String path);

			void setPath(Core::Uuid uuid, Core::String sourcePath);
	};
} // namespace Core