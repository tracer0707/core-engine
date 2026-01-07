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

		const Core::Uuid& getUuid() const { return _uuid; }
		void setUuid(Core::Uuid value) { _uuid = value; }
	};
} // namespace Core