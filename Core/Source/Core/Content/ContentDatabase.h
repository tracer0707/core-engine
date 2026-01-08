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

			std::map<String, Uuid> _pathToUuid;
			std::map<Uuid, String> _uuidToPath;

			String getRelativePath(String absolutePath);
			String getAbsolutePath(String relativePath);

		public:
			ContentDatabase(Application* app);
			~ContentDatabase();

			bool hasPath(Uuid uuid);
			bool hasUuid(String path);

			String getPath(Uuid uuid);
			Uuid getUuid(String path);

			void setPath(Uuid uuid, String path);

			void dump(Core::String path) const;
	};
} // namespace Core