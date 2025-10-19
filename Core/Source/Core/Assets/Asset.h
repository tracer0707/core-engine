#pragma once

#include "../Shared/Uuid.h"

namespace Core
{
	class Asset
	{
	private:
		Uuid _uuid;

	public:
		Asset();
		virtual ~Asset();
	};
}