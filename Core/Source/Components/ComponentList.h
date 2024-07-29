#pragma once

#include "../Config.h"
#include "../Shared/Hash.h"

namespace Core
{
#ifndef COMPONENT_TRANSFORM
	#define COMPONENT_TRANSFORM Hash::getHash("Transform")
#endif

#ifndef COMPONENT_MESHRENDERER
	#define COMPONENT_MESHRENDERER Hash::getHash("MeshRenderer")
#endif

#ifndef COMPONENT_CAMERA
	#define COMPONENT_CAMERA Hash::getHash("Camera")
#endif
}