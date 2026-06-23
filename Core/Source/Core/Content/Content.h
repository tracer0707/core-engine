#pragma once

#include "../Shared/Uuid.h"
#include "ContentType.h"

namespace Core
{
	class Content
	{
		private:
			Core::Uuid _uuid = Core::Uuid::Empty;

		public:
			Content();
			virtual ~Content();

			virtual ContentType getContentType() const = 0;

			const Core::Uuid& getUuid() const { return _uuid; }
			void setUuid(Core::Uuid value) { _uuid = value; }
	};
} // namespace Core