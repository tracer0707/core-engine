#pragma once

#include "Inspector.h"

namespace Core
{
	class MeshRenderer;
	class ContentManager;
} // namespace Core

namespace Editor
{
	class MeshRendererInspector : public Inspector
	{
		private:
			Core::MeshRenderer* _meshRenderer = nullptr;

		public:
			MeshRendererInspector(Core::MeshRenderer* meshRenderer, Core::EventHandler* eventHandler, Core::ContentManager* contentManager);
			virtual ~MeshRendererInspector();

			virtual void build();
	};
} // namespace Editor