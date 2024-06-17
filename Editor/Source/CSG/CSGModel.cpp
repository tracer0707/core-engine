#include "CSGModel.h"

#include <Components/Transform.h>

namespace Editor
{
	CSGModel::CSGModel()
	{
		transform = new Core::Transform();
	}

	CSGModel::~CSGModel()
	{
		delete transform;
		transform = nullptr;
	}
}