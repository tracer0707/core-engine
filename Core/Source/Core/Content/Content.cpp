#include "Content.h"

namespace Core
{
    Content::Content()
	{
		_uuid = Uuid::create();
	}

	Content::~Content()
	{
	}
}