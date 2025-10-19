#include "Asset.h"

namespace Core
{
	Asset::Asset()
	{
		_uuid = Uuid::create();
	}

	Asset::~Asset()
	{
	}
}