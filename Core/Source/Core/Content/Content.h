#pragma once

#include "../Shared/Uuid.h"

namespace Core
{
	class Content
	{
		private:
			Core::Uuid _uuid;

		public:
			Content();
			virtual ~Content();

			virtual int getContentType() = 0;

			const Core::Uuid& getUuid() const { return _uuid; }
			void setUuid(Core::Uuid value) { _uuid = value; }
	};
} // namespace Core