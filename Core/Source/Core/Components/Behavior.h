#pragma once

#include "Component.h"

namespace Core
{
	class Object;
	class Script;

	class Behavior : public Component
	{
		private:
			Script* _script = nullptr;

		public:
			Behavior(Object* owner);
			virtual ~Behavior();

			virtual ComponentType getComponentType() const { return ComponentType::Behavior; }

			Script* getScript() const { return _script; }
			void setScript(Script* script) { _script = script; }

			virtual void init();
			virtual void update(float& dt);
	};
} // namespace Core