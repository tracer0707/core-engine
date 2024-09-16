#pragma once

#include "Modifier.h"

namespace Editor
{
	class CSGModifier : public Modifier
	{
	private:
	public:
		CSGModifier();
		virtual ~CSGModifier();

		static UString NAME;

		virtual void init(Core::Scene* scene);
		virtual void update();
		virtual void render();
	};
}