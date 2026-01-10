#pragma once

#include <map>

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
			Core::String _filePath = Core::String::Empty;

			std::map<String, Uuid> _pathToUuid;
			std::map<Uuid, String> _uuidToPath;

			String getRelativePath(String absolutePath);
			String getAbsolutePath(String relativePath);

			void setApplication(Application* value) { _app = value; }
			void setFilePath(Core::String value) { _filePath = value; }

			static ContentDatabase _singleton;

		public:
			static ContentDatabase* singleton() { return &_singleton; }

			bool hasPath(Uuid uuid);
			bool hasUuid(String path);

			String getPath(Uuid uuid);
			Uuid getUuid(String path);

			void setPath(Uuid uuid, String path);

			void load();
			void save() const;
	};
} // namespace Core