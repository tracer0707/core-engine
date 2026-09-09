#pragma once

#include "../Shared/String.h"

#include "Content.h"

namespace Core
{
	class Script : public Content
	{
		friend class ContentManager;

		private:
			Script(String source);
			~Script();

			String _source = String::Empty;

		public:
			virtual ContentType getContentType() const { return ContentType::Script; }

			const String& getSource() const { return _source; }
	};
} // namespace Core