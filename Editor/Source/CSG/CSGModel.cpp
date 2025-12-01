#include "CSGModel.h"

#include <algorithm>

#include <carve/interpolator.hpp>
#include <carve/csg_triangulator.hpp>
#include <carve/carve.hpp>
#include <carve/csg.hpp>
#include <carve/input.hpp>

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

#include "../System/ContentLoader.h"
#include "CSGBrush.h"
#include "CSGBrushCube.h"

namespace Editor
{
    Core::Material* CSGModel::_defaultMaterial = nullptr;

    CSGModel::CSGModel(Core::Renderer* renderer, Core::Scene* scene, ContentLoader* contentLoader)
    {
        _renderer = renderer;
        _scene = scene;
        _contentLoader = contentLoader;
        _object = _scene->createObject();
        _meshRenderer = _object->addComponent<Core::MeshRenderer*>();

        _nullBrush = new CSGBrush(this);

        if (_defaultMaterial == nullptr) _defaultMaterial = _contentLoader->getContentManager()->createMaterial();
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
            _contentLoader->getContentManager()->destroy(currentMesh);
        }

        _meshRenderer->setMesh(nullptr);

        carve::csg::CSG csg;

        carve::interpolate::FaceVertexAttr<CSGBrush::uv_t> fv_uv;
        carve::interpolate::FaceAttr<Core::Material*> f_material;
        carve::interpolate::FaceAttr<int> f_layer;
        carve::interpolate::FaceAttr<bool> f_castShadows;
        carve::interpolate::FaceAttr<bool> f_smoothNormals;
        carve::interpolate::FaceAttr<Core::Uuid> f_brushId;

        carve::poly::Polyhedron* csgGeom = nullptr;

        _nullBrush->rebuild();

        // Bind attributes
        for (auto brush : _brushes)
        {
            brush->rebuild();
            brush->bind(&fv_uv, &f_material, &f_layer, &f_castShadows, &f_smoothNormals, &f_brushId);
        }

        fv_uv.installHooks(csg);
        f_material.installHooks(csg);
        f_layer.installHooks(csg);
        f_castShadows.installHooks(csg);
        f_smoothNormals.installHooks(csg);
        f_brushId.installHooks(csg);

        csg.hooks.registerHook(new carve::csg::CarveTriangulatorWithImprovement(), carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);

        // Compute CSG
        for (auto* brush : _brushes)
        {
            glm::mat4x4 mtx = brush->getTransform()->getTransformMatrix();
            glm::quat nrmMtx = brush->getTransform()->getRotation();

            carve::poly::Polyhedron* brushPtr = brush->getBrushPtr();
            carve::poly::Polyhedron* prevCSG = csgGeom;
            carve::csg::CSG::OP op = carve::csg::CSG::OP::UNION;

            if (brush->getBrushOperation() == CSGBrush::BrushOperation::Subtract) op = carve::csg::CSG::OP::A_MINUS_B;

            try
            {
                if (csgGeom == nullptr)
                    csgGeom = csg.compute(_nullBrush->getBrushPtr(), brushPtr, op);
                else
                    csgGeom = csg.compute(csgGeom, brushPtr, op);
            }
            catch (carve::exception e)
            {
                prevCSG = nullptr;
            }

            if (prevCSG != nullptr && prevCSG != _nullBrush->getBrushPtr()) delete prevCSG;
        }

        if (csgGeom == nullptr) return;

        // Build meshes
        Core::AxisAlignedBox aab = Core::AxisAlignedBox();

        for (unsigned long long i = 0; i < csgGeom->faces.size(); ++i)
        {
            auto* f = &csgGeom->faces[i];

            Core::Material* mat = _defaultMaterial;
            int layer = 0;
            bool castShadows = true;
            bool smoothNormals = false;
            Core::Uuid brushId = Core::Uuid::empty;

            if (f_material.hasAttribute(f))
            {
                Core::Material* mt = f_material.getAttribute(f);
                if (mt != nullptr) mat = mt;
            }

            if (f_layer.hasAttribute(f)) layer = f_layer.getAttribute(f);
            if (f_castShadows.hasAttribute(f)) castShadows = f_castShadows.getAttribute(f);
            if (f_smoothNormals.hasAttribute(f)) smoothNormals = f_smoothNormals.getAttribute(f);
            if (f_brushId.hasAttribute(f)) brushId = f_brushId.getAttribute(f);

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

            for (unsigned long long j = 0; j < 3; ++j)
            {
                carve::geom3d::Vector v = f->vertex(j)->v;
                CSGBrush::uv_t uv = CSGBrush::uv_t(0, 0);

                if (fv_uv.hasAttribute(f, j)) uv = fv_uv.getAttribute(f, j);

                Core::Vertex vtx{};

                vtx.position = glm::vec3((float)v.x, (float)v.y, (float)v.z);
                vtx.uv = glm::vec2(uv.u);
                vtx.color = Core::Color(1.0f, 1.0f, 1.0f, 1.0f);

                aab.merge(vtx.position);

                subMesh->vertices.add(vtx);
                subMesh->brushIds.add(brushId);
            }
        }

        if (csgGeom != nullptr && csgGeom != _nullBrush->getBrushPtr()) delete csgGeom;

        Core::Mesh* mesh = _contentLoader->getContentManager()->createMesh(_subMeshes.size());
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

    CSGBrush* CSGModel::findBrush(Core::Uuid brushId)
    {
        for (auto it : _brushes)
        {
            if (it->getId() == brushId) return it;
        }

        return nullptr;
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

    Core::Uuid CSGModel::getBrushId(const Core::SubMesh* subMesh, unsigned int vertexId)
    {
        for (auto& sm : _subMeshes)
        {
            if (subMesh == sm.second->subMesh)
            {
                if (sm.second->brushIds.count() > vertexId)
                {
                    return sm.second->brushIds.get(vertexId);
                }
            }
        }

        return Core::Uuid::empty;
    }
} // namespace Editor