#include "CSGModifier.h"

namespace Editor
{
	UString CSGModifier::NAME = "CSG";

	CSGModifier::CSGModifier() : Modifier(NAME)
	{
	}

	CSGModifier::~CSGModifier()
	{
	}

	void CSGModifier::init(Core::Scene* scene)
	{
		Modifier::init(scene);
	}

	void CSGModifier::update()
	{
	}

	void CSGModifier::render()
	{
	}
}