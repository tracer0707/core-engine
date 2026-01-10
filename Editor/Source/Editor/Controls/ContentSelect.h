#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/Uuid.h>

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
			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

			Core::Content* _content = nullptr;
			int _contentType = INT_MAX;

			Core::String getContentName() const;

		public:
			ContentSelect();
			virtual ~ContentSelect();

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual int getControlType() const;
			virtual void update();

			Core::Content* getContent() const { return _content; }
			void setContent(Core::Content* value) { _content = value; }

			int getContentType() const { return _contentType; }
			void setContentType(int value) { _contentType = value; }
	};
}