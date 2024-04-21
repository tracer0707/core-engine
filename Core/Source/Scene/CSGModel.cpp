#include "CSGModel.h"

#include "../Components/Transform.h"

namespace Core
{
	CSGModel::CSGModel()
	{
		transform = new Transform();
	}

	CSGModel::~CSGModel()
	{
		delete transform;
		transform = nullptr;
	}
}