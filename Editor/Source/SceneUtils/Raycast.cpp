#include "Raycast.h"

#include "../Editor/Modifiers/ModifierManager.h"
#include "../Editor/Modifiers/CSGModifier.h"
#include "../CSG/CSGModel.h"
#include "../CSG/CSGBrush.h"

namespace Editor
{
	bool Raycast::hitTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit)
	{
		bool hit = false;

		if (ModifierManager::singleton()->getCurrentModifierName() == CSGModifier::NAME)
		{
			return csgTest(scene, ray, outHit);
		}

		return false;
	}

	bool Raycast::csgTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit)
	{
		

		return false;
	}
}