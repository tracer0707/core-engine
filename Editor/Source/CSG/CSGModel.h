#pragma once

#include <vector>
#include <unordered_map>

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
					Core::List<CSGBrush*> brushes;
					Core::List<size_t> faceIds;
			};

			Core::List<CSGBrush*> _brushes;
			std::unordered_map<Core::Material*, SubMeshInfo*> _subMeshes;

			Core::Renderer* _renderer = nullptr;
			Core::Scene* _scene = nullptr;
			Core::Object* _object = nullptr;
			Core::MeshRenderer* _meshRenderer = nullptr;
			Core::ContentManager* _contentManager = nullptr;

			CSGBrush* _nullBrush = nullptr;

		public:
			CSGModel(Core::Renderer* renderer, Core::Scene* scene, Core::ContentManager* contentManager);
			~CSGModel();

			Core::Scene* getScene() const { return _scene; }
			Core::Object* getObject() const { return _object; }
			Core::MeshRenderer* getMeshRenderer() const { return _meshRenderer; }

			Core::ContentManager* getContentManager() const { return _contentManager; }

			CSGBrushCube* createCubeBrush();

			const Core::List<CSGBrush*>& getBrushes() const { return _brushes; }
			bool removeBrush(CSGBrush* value);

			void findBrush(const Core::SubMesh* subMesh, int vertexId, CSGBrush** outCsgBrush, size_t* outFaceId);

			void rebuild();
	};
} // namespace Editor