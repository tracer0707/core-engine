#pragma once

#include "../Shared/String.h"

#include "Content.h"

namespace Core
{
	class Script : public Content
	{
		friend class ContentManager;

		private:
			Script(String sourceCode);
			~Script();

			String _sourceCode = String::Empty;

		public:
			virtual ContentType getContentType() const { return ContentType::Script; }
	};
} // namespace Core