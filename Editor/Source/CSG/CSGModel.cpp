#include "CSGModel.h"

#include <algorithm>

#include <carve/interpolator.hpp>
#include <carve/csg_triangulator.hpp>
#include <carve/carve.hpp>
#include <carve/csg.hpp>
#include <carve/input.hpp>

#include <Scene/Object.h>
#include <Components/Transform.h>
#include <Assets/Material.h>
#include <Assets/Mesh.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/Renderer.h>

#include "CSGBrush.h"
#include "CSGBrushCube.h"

namespace Editor
{
    Core::Material* CSGModel::defaultMaterial = nullptr;

    struct SubMeshInfo
    {
        Core::List<Core::Vertex> vertices;
        Core::List<UInt32> indices;
    };

	CSGModel::CSGModel()
	{
        object = new Core::Object();
		transform = object->addComponent<Core::Transform*>();

        nullBrush = new CSGBrushCube();
        if (defaultMaterial == nullptr)
            defaultMaterial = new Core::Material();
	}

	CSGModel::~CSGModel()
	{
		delete object;
        object = nullptr;
		transform = nullptr;

        if (nullBrush != nullptr)
            delete nullBrush;

        nullBrush = nullptr;
	}

	void CSGModel::rebuild()
	{
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
        
        for (auto brush : csgBrushes)
        {
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
        for (auto brush : csgBrushes)
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
                //Debug::log("CSG build error", Debug::DbgColorRed);
            }

            if (prevCSG != nullptr && prevCSG != nullBrush->getBrushPtr())
                delete prevCSG;
        }

        if (csgGeom == nullptr)
            return;

        //Build meshes
        std::map<Core::Material*, SubMeshInfo*> subMeshes;

        for (size_t i = 0; i < csgGeom->faces.size(); ++i)
        {
            auto* f = &csgGeom->faces[i];

            Core::Material* mat = defaultMaterial;
            int layer = 0;
            bool castShadows = true;
            bool smoothNormals = false;
            size_t brushId = 0;

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

            auto it = subMeshes.find(mat);
            if (it != subMeshes.end())
            {
                subMesh = it->second;
                break;
            }
            else
            {
                subMesh = new SubMeshInfo();
                subMeshes[mat] = subMesh;
            }

            std::vector<Core::Vertex> verts;
            std::vector<UInt32> inds;

            for (size_t j = 0; j < 3; ++j)
            {
                carve::geom3d::Vector v = f->vertex(j)->v;
                CSGBrush::uv_t uv = CSGBrush::uv_t(0, 0);

                if (fv_uv.hasAttribute(f, j))
                    uv = fv_uv.getAttribute(f, j);

                Core::Vertex vtx;
                vtx.position[0] = (float)v.x;
                vtx.position[1] = (float)v.y;
                vtx.position[2] = (float)v.z;

                vtx.uv[0] = uv.u;
                vtx.uv[1] = uv.v;

                subMesh->vertices.add(vtx);
            }
        }

        if (csgGeom != nullptr && csgGeom != nullBrush->getBrushPtr())
            delete csgGeom;

        Core::SubMesh** _subMeshes = new Core::SubMesh*[subMeshes.size()];
        mesh = new Core::Mesh(_subMeshes, subMeshes.size());

        for (int i = 0; i < subMeshes.size(); ++i)
        {
            auto it = subMeshes.begin();
            std::advance(it, i);

            const Core::VertexBuffer* _vertexBuffer = Core::Renderer::singleton()->createBuffer(it->second->vertices.ptr(), it->second->vertices.count(), nullptr, 0);
            Core::SubMesh* subMesh = new Core::SubMesh(_vertexBuffer);
            subMesh->setMaterial(it->first);

            _subMeshes[i] = subMesh;
        }
	}
}