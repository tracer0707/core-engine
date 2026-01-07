#pragma once

#include <vector>
#include <map>

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>
#include <Core/Shared/Uuid.h>

namespace Core
{
	class Object;
	class MeshRenderer;
	class Material;
	class SubMesh;
	class Scene;
	class Renderer;
	class ContentManager;
	struct Vertex;
} // namespace Core

namespace Editor
{
	class CSGBrush;
	class CSGBrushCube;

	class CSGModel
	{
		private:
			struct SubMeshInfo
			{
					Core::SubMesh* subMesh = nullptr;
					Core::List<Core::Vertex> vertices;
					Core::List<Core::Uuid> brushIds;
			};

			Core::String _name = "";
			Core::List<CSGBrush*> _brushes;
			std::map<Core::Material*, SubMeshInfo*> _subMeshes;

			Core::Renderer* _renderer = nullptr;
			Core::Scene* _scene = nullptr;
			Core::Object* _object = nullptr;
			Core::MeshRenderer* _meshRenderer = nullptr;
			Core::ContentManager* _contentManager = nullptr;

			static Core::Material* _defaultMaterial;

			CSGBrush* _nullBrush = nullptr;

		public:
			CSGModel(Core::Renderer* renderer, Core::Scene* scene, Core::ContentManager* contentManager);
			~CSGModel();

			Core::String getName() { return _name; }
			void setName(Core::String value) { _name = value; }

			Core::Object* getObject() { return _object; }
			Core::MeshRenderer* getMeshRenderer() { return _meshRenderer; }

			Core::ContentManager* getContentManager() { return _contentManager; }

			CSGBrushCube* createCubeBrush();

			int getNumBrushes() { return _brushes.count(); }
			CSGBrush* getBrush(int index) { return _brushes.get(index); }
			CSGBrush* findBrush(Core::Uuid brushId);
			bool removeBrush(CSGBrush* value);

			Core::Uuid getBrushId(const Core::SubMesh* subMesh, unsigned int vertexId);

			void rebuild();
	};
} // namespace Editor