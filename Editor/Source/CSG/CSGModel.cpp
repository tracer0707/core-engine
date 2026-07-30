#include "CSGModel.h"

#include <algorithm>

#include <manifold/mesh.h>
#include <manifold/manifold.h>
#include <optional>

#include <Core/Scene/Scene.h>
#include <Core/Scene/Object.h>
#include <Core/Components/Transform.h>
#include <Core/Components/MeshRenderer.h>
#include <Core/Content/ContentManager.h>
#include <Core/Content/Material.h>
#include <Core/Content/Mesh.h>
#include <Core/Renderer/VertexBuffer.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Color.h>

#include "CSGBrush.h"
#include "CSGBrushCube.h"

namespace Editor
{
	CSGModel::CSGModel(Core::Renderer* renderer, Core::Scene* scene, Core::ContentManager* contentManager)
	{
		_renderer = renderer;
		_scene = scene;
		_contentManager = contentManager;
		_object = _scene->createObject();
		_meshRenderer = _object->addComponent<Core::MeshRenderer*>();

		_nullBrush = new CSGBrush(this);
	}

	CSGModel::~CSGModel()
	{
		_scene->removeObject(_object);

		_object = nullptr;
		_meshRenderer = nullptr;

		if (_nullBrush != nullptr) delete _nullBrush;

		_nullBrush = nullptr;
	}

	void CSGModel::rebuild()
	{
		for (auto& sm : _subMeshes)
		{
			sm.second->brushIds.clear();
			sm.second->vertices.clear();
			delete sm.second;
		}

		_subMeshes.clear();

		Core::Mesh* currentMesh = _meshRenderer->getMesh();
		if (currentMesh != nullptr)
		{
			_contentManager->destroy(currentMesh);
		}

		_meshRenderer->setMesh(nullptr);

		std::optional<manifold::Manifold> csgGeom;

		_nullBrush->rebuild();

		std::unordered_map<uint32_t, CSGBrush*> originalMap;
		for (auto brush : _brushes)
		{
			brush->rebuild();
			if (brush->getBrushPtr() != nullptr)
			{
				originalMap[brush->getOriginalId()] = brush;
			}
		}

		for (auto* brush : _brushes)
		{
			manifold::Manifold* brushPtr = brush->getBrushPtr();
			if (brushPtr == nullptr) continue;

			try
			{
				if (!csgGeom.has_value())
				{
					if (_nullBrush->getBrushPtr() != nullptr)
					{
						csgGeom = *_nullBrush->getBrushPtr();
						if (brush->getBrushOperation() == CSGBrush::BrushOperation::Subtract)
							csgGeom = csgGeom.value() - (*brushPtr);
						else
							csgGeom = csgGeom.value() + (*brushPtr);
					}
					else
					{
						csgGeom = *brushPtr;
					}
				}
				else
				{
					if (brush->getBrushOperation() == CSGBrush::BrushOperation::Subtract)
						csgGeom = csgGeom.value() - (*brushPtr);
					else
						csgGeom = csgGeom.value() + (*brushPtr);
				}
			}
			catch (const std::exception&)
			{
				csgGeom.reset();
			}
		}

		if (!csgGeom.has_value()) return;

		// Build meshes
		Core::AxisAlignedBox aab = Core::AxisAlignedBox();

		manifold::MeshGL out = csgGeom->GetMeshGL();

		std::vector<size_t> runStart;
		for (size_t r = 0; r < out.runIndex.size(); ++r)
		{
			size_t triStart = out.runIndex[r] / 3;
			runStart.push_back(triStart);
		}

		size_t triCount = out.NumTri();
		for (size_t t = 0; t < triCount; ++t)
		{
			size_t triIdxStart = t;
			size_t run = 0;
			for (size_t r = 0; r + 1 < runStart.size(); ++r)
			{
				if (triIdxStart >= runStart[r] && triIdxStart < runStart[r+1]) { run = r; break; }
				if (r + 1 == runStart.size() - 1) run = r + 1;
			}

			uint32_t original = 0;
			if (run < out.runOriginalID.size()) original = out.runOriginalID[run];

			CSGBrush* srcBrush = nullptr;
			if (originalMap.find(original) != originalMap.end()) srcBrush = originalMap[original];

			Core::Material* mat = nullptr;
			Core::Uuid brushUuid = Core::Uuid::Empty;
			size_t faceId = 0;
			bool castShadows = true;
			bool smoothNormals = false;

			if (srcBrush != nullptr)
			{
				if (t < out.faceID.size()) faceId = out.faceID[t];
				mat = srcBrush->getMaterial((int)faceId);
				brushUuid = srcBrush->getId();
				smoothNormals = srcBrush->getSmoothNormals((int)faceId);
				castShadows = srcBrush->getCastShadows();
			}

			SubMeshInfo* subMesh = nullptr;
			auto it = _subMeshes.find(mat);
			if (it != _subMeshes.end())
			{
				subMesh = it->second;
			}
			else
			{
				subMesh = new SubMeshInfo();
				_subMeshes[mat] = subMesh;
			}

			for (size_t vi = 0; vi < 3; ++vi)
			{
				uint32_t vertIdx = out.triVerts[3 * t + vi];
				size_t propOff = (size_t)vertIdx * out.numProp;

				Core::Vertex vtx{};
				if (propOff + 2 < out.vertProperties.size())
				{
					vtx.position = glm::vec3((float)out.vertProperties[propOff + 0], (float)out.vertProperties[propOff + 1], (float)out.vertProperties[propOff + 2]);
				}
				if (out.numProp > 3 && propOff + 4 < out.vertProperties.size())
				{
					vtx.uv = glm::vec2((float)out.vertProperties[propOff + 3], (float)out.vertProperties[propOff + 4]);
				}

				vtx.color = Core::Color(1.0f, 1.0f, 1.0f, 1.0f);

				aab.merge(vtx.position);

				subMesh->vertices.add(vtx);
				subMesh->brushIds.add(brushUuid);
				subMesh->faceIds.add(faceId);
			}
		}

		Core::Mesh* mesh = _contentManager->createMesh(_subMeshes.size());
		Core::SubMesh** subMeshes = mesh->getSubMeshes();
		mesh->setBoundingBox(aab);
		_meshRenderer->setMesh(mesh);

		for (int i = 0; i < _subMeshes.size(); ++i)
		{
			auto it = _subMeshes.begin();
			std::advance(it, i);

			subMeshes[i]->setMaterial(it->first);
			subMeshes[i]->updateVertexBuffer(it->second->vertices.ptr(), it->second->vertices.count(), nullptr, 0);

			it->second->vertices.clear();
			it->second->subMesh = subMeshes[i];
		}
	}

	CSGBrushCube* CSGModel::createCubeBrush()
	{
		CSGBrushCube* brush = new CSGBrushCube(this);
		_brushes.add(brush);

		return brush;
	}

	bool CSGModel::removeBrush(CSGBrush* value)
	{
		if (_brushes.contains(value))
		{
			_brushes.remove(value);
			delete value;

			return true;
		}

		return false;
	}

	CSGBrush* CSGModel::findBrushByUuid(Core::Uuid brushId)
	{
		for (auto it : _brushes)
		{
			if (it->getId() == brushId) return it;
		}

		return nullptr;
	}

	void CSGModel::findBrush(const Core::SubMesh* subMesh, unsigned int vertexId, CSGBrush** outCsgBrush, size_t* outFaceId)
	{
		for (auto& sm : _subMeshes)
		{
			if (subMesh == sm.second->subMesh)
			{
				if (sm.second->brushIds.count() > vertexId)
				{
					*outCsgBrush = findBrushByUuid(sm.second->brushIds.get(vertexId));
					*outFaceId = sm.second->faceIds.get(vertexId);
					return;
				}
			}
		}

		*outCsgBrush = nullptr;
		*outFaceId = -1;
	}
} // namespace Editor