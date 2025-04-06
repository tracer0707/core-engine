#include "CSGModel.h"

#include <algorithm>

#include <carve/interpolator.hpp>
#include <carve/csg_triangulator.hpp>
#include <carve/carve.hpp>
#include <carve/csg.hpp>
#include <carve/input.hpp>

#include <Scene/Object.h>
#include <Components/Transform.h>
#include <Components/MeshRenderer.h>
#include <Assets/Material.h>
#include <Assets/Mesh.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/Renderer.h>

#include "../Shared/Layers.h"

#include "CSGBrush.h"
#include "CSGBrushCube.h"

namespace Editor
{
    Core::Material* CSGModel::defaultMaterial = nullptr;

	CSGModel::CSGModel()
	{
        object = new Core::Object();
        meshRenderer = object->addComponent<Core::MeshRenderer*>();
		transform = object->addComponent<Core::Transform*>();

        Core::BitSet& objectFlags = object->getFlags();
        objectFlags.setBit(LAYER_CSG, true);

        nullBrush = new CSGBrush();

        if (defaultMaterial == nullptr)
            defaultMaterial = new Core::Material();
	}

	CSGModel::~CSGModel()
	{
		delete object;
        object = nullptr;
		transform = nullptr;
        meshRenderer = nullptr;

        if (nullBrush != nullptr)
            delete nullBrush;

        nullBrush = nullptr;
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

        Core::Mesh* currentMesh = meshRenderer->getMesh();
        if (currentMesh != nullptr) delete currentMesh;

        meshRenderer->setMesh(nullptr);

        carve::csg::CSG csg;

        carve::interpolate::FaceVertexAttr<CSGBrush::uv_t> fv_uv;
        carve::interpolate::FaceAttr<Core::Material*> f_material;
        carve::interpolate::FaceAttr<int> f_layer;
        carve::interpolate::FaceAttr<bool> f_castShadows;
        carve::interpolate::FaceAttr<bool> f_smoothNormals;
        carve::interpolate::FaceAttr<size_t> f_brushId;

        carve::poly::Polyhedron* csgGeom = nullptr;

        nullBrush->rebuild();

        //Bind attributes
        for (auto brush : _csgBrushes)
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

        //Compute CSG
        for (auto* brush : _csgBrushes)
        {
            glm::mat4x4 mtx = brush->getTransform()->getTransformMatrix();
            glm::quat nrmMtx = brush->getTransform()->getRotation();

            carve::poly::Polyhedron* brushPtr = brush->getBrushPtr();
            carve::poly::Polyhedron* prevCSG = csgGeom;
            carve::csg::CSG::OP op = carve::csg::CSG::OP::UNION;

            if (brush->getBrushOperation() == CSGBrush::BrushOperation::Subtract)
                op = carve::csg::CSG::OP::A_MINUS_B;

            try
            {
                if (csgGeom == nullptr)
                    csgGeom = csg.compute(nullBrush->getBrushPtr(), brushPtr, op);
                else
                    csgGeom = csg.compute(csgGeom, brushPtr, op);
            }
            catch (carve::exception e)
            {
                prevCSG = nullptr;
            }

            if (prevCSG != nullptr && prevCSG != nullBrush->getBrushPtr())
                delete prevCSG;
        }

        if (csgGeom == nullptr)
            return;

        //Build meshes
        Core::AxisAlignedBox aab = Core::AxisAlignedBox();

        for (unsigned long long i = 0; i < csgGeom->faces.size(); ++i)
        {
            auto* f = &csgGeom->faces[i];

            Core::Material* mat = defaultMaterial;
            int layer = 0;
            bool castShadows = true;
            bool smoothNormals = false;
            unsigned long long brushId = 0;

            if (f_material.hasAttribute(f))
            {
                Core::Material* mt = f_material.getAttribute(f);
                if (mt != nullptr)
                    mat = mt;
            }

            if (f_layer.hasAttribute(f))
                layer = f_layer.getAttribute(f);

            if (f_castShadows.hasAttribute(f))
                castShadows = f_castShadows.getAttribute(f);

            if (f_smoothNormals.hasAttribute(f))
                smoothNormals = f_smoothNormals.getAttribute(f);

            if (f_brushId.hasAttribute(f))
                brushId = f_brushId.getAttribute(f);

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

                if (fv_uv.hasAttribute(f, j))
                    uv = fv_uv.getAttribute(f, j);

                Core::Vertex vtx{};

                vtx.position[0] = (float)v.x;
                vtx.position[1] = (float)v.y;
                vtx.position[2] = (float)v.z;

                vtx.uv[0] = uv.u;
                vtx.uv[1] = uv.v;

                vtx.color[0] = 1.0f;
                vtx.color[1] = 1.0f;
                vtx.color[2] = 1.0f;
                vtx.color[3] = 1.0f;

                aab.merge(vtx.getPosition());

                subMesh->vertices.add(vtx);
                subMesh->brushIds.add(brushId);
            }
        }

        if (csgGeom != nullptr && csgGeom != nullBrush->getBrushPtr())
            delete csgGeom;

        Core::SubMesh** subMeshes = new Core::SubMesh*[_subMeshes.size()];
        Core::Mesh* mesh = new Core::Mesh(subMeshes, _subMeshes.size());
        mesh->setBoundingBox(aab);
        meshRenderer->setMesh(mesh);

        for (int i = 0; i < _subMeshes.size(); ++i)
        {
            auto it = _subMeshes.begin();
            std::advance(it, i);

            const Core::VertexBuffer* _vertexBuffer = Core::Renderer::singleton()->createBuffer(it->second->vertices.ptr(), it->second->vertices.count(), nullptr, 0);

            Core::SubMesh* subMesh = new Core::SubMesh(_vertexBuffer);
            subMesh->setMaterial(it->first);

            subMeshes[i] = subMesh;

            it->second->vertices.clear();
            it->second->subMesh = subMesh;
        }
	}

    unsigned long long CSGModel::getBrushId(const Core::SubMesh* subMesh, unsigned int vertexId)
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

        return -1;
    }
}