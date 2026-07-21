#pragma once

#include <functional>
#include <climits>

#include <Core/Shared/String.h>
#include <Core/Shared/Uuid.h>
#include <Core/Content/ContentType.h>

#include "Control.h"

namespace Core
{
	class Content;
}

namespace Editor
{
	class ContentSelect : public Control
	{
		private:
			Core::Content* _content = nullptr;
			Core::ContentType _contentType = Core::ContentType::None;

			std::function<void(Core::Content*)> _onContentChanged = nullptr;

			Core::String getContentName() const;

		public:
			ContentSelect();
			virtual ~ContentSelect();

			virtual ControlType getControlType() const { return ControlType::ContentSelect; }
			virtual void update();

			Core::Content* getContent() const { return _content; }
			void setContent(Core::Content* value) { _content = value; }

			Core::ContentType getContentType() const { return _contentType; }
			void setContentType(Core::ContentType value) { _contentType = value; }

			void setOnContentChanged(std::function<void(Core::Content*)> value) { _onContentChanged = value; }
	};
}