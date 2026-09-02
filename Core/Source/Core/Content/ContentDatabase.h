#pragma once

#include <map>
#include "../Shared/Path.h"

#include "../Shared/Uuid.h"
#include "../Shared/String.h"

namespace Core
{
	class Application;

	class ContentDatabase
	{
			friend class ContentManager;

		private:
			ContentDatabase();
			~ContentDatabase();

			Application* _app = nullptr;
			fs::path _filePath;

			std::map<fs::path, Uuid> _pathToUuid;
			std::map<Uuid, fs::path> _uuidToPath;

			fs::path getRelativePath(const fs::path& absolutePath);
			fs::path getAbsolutePath(const fs::path& relativePath);

			void setApplication(Application* value) { _app = value; }
			void setFilePath(const fs::path& value) { _filePath = value; }

			static ContentDatabase _singleton;

		public:
			static ContentDatabase* singleton() { return &_singleton; }

			bool hasPath(Uuid uuid);
			bool hasUuid(const fs::path& path);

			fs::path getPath(Uuid uuid);
			Uuid getUuid(const fs::path& path);

			void setPath(Uuid uuid, const fs::path& path);

			void load();
			void save() const;
	};
} // namespace Core