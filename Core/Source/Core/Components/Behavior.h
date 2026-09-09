#pragma once

#include "Component.h"

namespace Core
{
	class Object;
	class Script;
	class BehaviorManager;
	class BehaviorInstance;

	class Behavior : public Component
	{
		private:
			Script* _script = nullptr;
			BehaviorManager* _behaviorManager = nullptr;
			BehaviorInstance* _behaviorInstance = nullptr;

		public:
			Behavior(Object* owner, BehaviorManager* behaviorManager);
			virtual ~Behavior();

			virtual ComponentType getComponentType() const { return ComponentType::Behavior; }

			Script* getScript() const { return _script; }
			void setScript(Script* script);

			virtual void update(float& dt);
	};
} // namespace Core