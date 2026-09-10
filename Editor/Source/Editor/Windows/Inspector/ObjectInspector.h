#pragma once

#include "Inspector.h"

namespace Core
{
	class Object;
	class ContentManager;
} // namespace Core

namespace Editor
{
	class ObjectInspector : public Inspector
	{
		private:
			Core::Object* _object = nullptr;
			
		public:
			ObjectInspector(Core::Object* object, Core::EventHandler* eventHandler, Core::ContentManager* contentManager);
			virtual ~ObjectInspector();

			virtual void build();
	};
} // namespace Editor