#pragma once

#include "../Shared/Uuid.h"

namespace Core
{
    class Content
	{
	private:
		Uuid _uuid;

	public:
		Content();
        virtual ~Content();
	};
}