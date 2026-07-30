#include "CSGBrush.h"

#include "CSGModel.h"

#include <manifold/mesh.h>
#include <manifold/manifold.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Core/Math/Mathf.h>
#include <Core/Scene/Object.h>
#include <Core/Components/Transform.h>

namespace Editor
{
	CSGBrush::CSGBrush(CSGModel* parent)
	{
		this->parent = parent;

		transform = new Core::Transformable();
		Core::Transform* t = parent->getObject()->findComponent<Core::Transform*>();
		transform->setParent(t);
	}

	CSGBrush::~CSGBrush()
	{
		delete transform;
		transform = nullptr;

		destroy();
	}

	CSGBrush::uv_t operator*(double s, const CSGBrush::uv_t& t)
	{
		return CSGBrush::uv_t(t.u * s, t.v * s);
	}

	CSGBrush::uv_t& operator+=(CSGBrush::uv_t& t1, const CSGBrush::uv_t& t2)
	{
		t1.u += t2.u;
		t1.v += t2.v;
		return t1;
	}

	void CSGBrush::destroy()
	{
		if (brushPtr != nullptr)
		{
			delete brushPtr;
			brushPtr = nullptr;
		}
	}

	void CSGBrush::rebuild()
	{
		destroy();

		manifold::MeshGL mesh;
		mesh.numProp = 5; // x,y,z,u,v

		glm::mat4x4 mtx = glm::identity<glm::mat4x4>();
		mtx = transform->getTransformMatrix();

		originalId = manifold::Manifold::ReserveIDs(1);

		mesh.runIndex.push_back(0);
		mesh.runOriginalID.push_back(originalId);

		uint32_t vertIndex = 0;

		for (int i = 0; i < faces.count(); ++i)
		{
			FaceInfo& face = faces.get(i);

			size_t n = face.indices.count();
			if (n < 3) continue;

			std::vector<int> idx;
			for (size_t k = 0; k < n; ++k) idx.push_back(face.indices.get(k));

			std::vector<std::array<int,3>> tris;
			if (n == 3) tris.push_back({{0,1,2}});
			else if (n == 4) tris.push_back({{0,1,2}}), tris.push_back({{0,2,3}});
			else
			{
				for (size_t k = 1; k + 1 < n; ++k) tris.push_back({{0,(int)k,(int)(k+1)}});
			}

			glm::vec3 p0_world = mtx * glm::vec4(vertices.get(idx[0]), 1.0f);
			glm::vec3 p1_world = mtx * glm::vec4(vertices.get(idx[1]), 1.0f);
			glm::vec3 p2_world = mtx * glm::vec4(vertices.get(idx.size() > 2 ? idx[2] : idx[1]), 1.0f);

			glm::vec3 center_world = glm::vec3(0.0f);
			for (size_t ii = 0; ii < idx.size(); ++ii) center_world += glm::vec3(mtx * glm::vec4(vertices.get(idx[ii]), 1.0f));
			center_world /= (float)idx.size();

			glm::vec3 center_ns = glm::vec3(0.0f);
			for (size_t ii = 0; ii < idx.size(); ++ii) center_ns += transform->getRotation() * vertices.get(idx[ii]) + transform->getPosition();
			center_ns /= (float)idx.size();

			glm::vec3 e1 = p1_world - p0_world;
			glm::vec3 e2 = p2_world - p0_world;

			glm::vec3 e1_local = vertices.get(idx.size() > 1 ? idx[1] : idx[0]) - vertices.get(idx[0]);
			glm::vec3 e2_local = vertices.get(idx.size() > 2 ? idx[2] : idx[1]) - vertices.get(idx[0]);

			double lenU_local = glm::length(e1_local);
			double lenV_local = glm::length(e2_local);

			if (lenU_local < 1e-6) lenU_local = 1.0;
			if (lenV_local < 1e-6) lenV_local = 1.0;

			glm::vec3 fn = glm::cross(e1, e2);
			if (glm::length(fn) == 0.0f)
			{
				fn = glm::vec3(0.0f, 1.0f, 0.0f);
			}

			fn = glm::normalize(fn);
			glm::vec3 axisU = e1;
			if (glm::length(axisU) == 0.0f)
			{
				axisU = e2;
				if (glm::length(axisU) == 0.0f)
				{
					axisU = glm::vec3(1.0f, 0.0f, 0.0f);
				}
			}
			axisU = glm::normalize(axisU);
			glm::vec3 axisV = glm::cross(fn, axisU);
			if (glm::length(axisV) == 0.0f) axisV = glm::vec3(0.0f, 0.0f, 1.0f);
			axisV = glm::normalize(axisV);

			for (auto &t : tris)
			{
				for (int pi = 0; pi < 3; ++pi)
				{
					int corner = t[pi];
					glm::vec3 v = vertices.get(idx[corner]);
					glm::vec4 lp = glm::vec4(v, 1.0f);
					glm::vec3 p = mtx * lp;

					glm::vec3 p_world = p;
					glm::vec3 relp = p_world - center_world;
				
					double u = glm::dot(relp, axisU) * face.texCoordsScale.x;
					double vv = glm::dot(relp, axisV) * face.texCoordsScale.y;
					glm::vec2 uv = glm::vec2((float)u, (float)vv);

					uv.x = uv.x * face.texCoordsScale.x + face.texCoordsOffset.x;
					uv.y = uv.y * face.texCoordsScale.y + face.texCoordsOffset.y;
					uv = Core::Mathf::rotateUV(uv, Core::Mathf::fDeg2Rad * face.texCoordsRotation);

					mesh.vertProperties.push_back((float)p.x);
					mesh.vertProperties.push_back((float)p.y);
					mesh.vertProperties.push_back((float)p.z);
					mesh.vertProperties.push_back((float)uv.x);
					mesh.vertProperties.push_back((float)uv.y);

					mesh.triVerts.push_back(vertIndex);
					vertIndex++;
				}

				mesh.faceID.push_back(i);
			}
		}

		mesh.runIndex.push_back((uint32_t)mesh.triVerts.size());

		mesh.runTransform.resize(12);
		mesh.runTransform[0] = 1; mesh.runTransform[1] = 0; mesh.runTransform[2] = 0; mesh.runTransform[3] = 0;
		mesh.runTransform[4] = 0; mesh.runTransform[5] = 1; mesh.runTransform[6] = 0; mesh.runTransform[7] = 0;
		mesh.runTransform[8] = 0; mesh.runTransform[9] = 0; mesh.runTransform[10] = 1; mesh.runTransform[11] = 0;

		try
		{
			mesh.Merge();

			manifold::Manifold m(mesh);
			brushPtr = new manifold::Manifold(m);
		}
		catch (const std::exception& ex) {
			std::cout << "CSGBrush::rebuild: Manifold construction failed: " << ex.what() << "\n";
			brushPtr = nullptr;
		}
		catch (...) {
			std::cout << "CSGBrush::rebuild: Manifold construction threw unknown exception\n";
			brushPtr = nullptr;
		}
	}

	void CSGBrush::setBrushOperation(BrushOperation value)
	{
		brushOperation = value;
		rebuild();
	}

	Core::List<uint32_t> CSGBrush::getFlatIndices()
	{
		Core::List<uint32_t> inds;

		for (int i = 0; i < faces.count(); ++i)
		{
			for (int j = 0; j < faces.get(i).indices.count(); ++j)
			{
				inds.add(faces.get(i).indices.get(j));
			}
		}

		return inds;
	}

	void CSGBrush::setCastShadows(bool value)
	{
		castShadows = value;
		rebuild();
	}

	Core::Material* CSGBrush::getMaterial(int faceIndex)
	{
		if (faceIndex < faces.count()) return faces.get(faceIndex).material;

		return nullptr;
	}

	void CSGBrush::setMaterial(int faceIndex, Core::Material* value)
	{
		if (faceIndex < faces.count()) faces.get(faceIndex).material = value;
	}

	glm::vec2 CSGBrush::getUV(int faceIndex, int vertIndex)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);

			if (vertIndex < face.texCoords.count()) return face.texCoords.get(vertIndex);
		}

		return glm::vec2(0.0f);
	}

	void CSGBrush::setUV(int faceIndex, int vertIndex, glm::vec2 value)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);

			if (vertIndex < face.texCoords.count()) face.texCoords.get(vertIndex) = value;
		}
	}

	glm::vec2 CSGBrush::getUVScale(int faceIndex)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);
			return face.texCoordsScale;
		}

		return glm::vec2(1.0f);
	}

	void CSGBrush::setUVScale(int faceIndex, glm::vec2 value)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);
			face.texCoordsScale = value;
		}
	}

	glm::vec2 CSGBrush::getUVOffset(int faceIndex)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);
			return face.texCoordsOffset;
		}

		return glm::vec2(0.0f);
	}

	void CSGBrush::setUVOffset(int faceIndex, glm::vec2 value)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);
			face.texCoordsOffset = value;
		}
	}

	float CSGBrush::getUVRotation(int faceIndex)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);
			return face.texCoordsRotation;
		}

		return 0.0f;
	}

	void CSGBrush::setUVRotation(int faceIndex, float value)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);
			face.texCoordsRotation = value;
		}
	}

	bool CSGBrush::getSmoothNormals(int faceIndex)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);
			return face.smoothNormals;
		}

		return false;
	}

	void CSGBrush::setSmoothNormals(int faceIndex, bool value)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);
			face.smoothNormals = value;
		}
	}
} // namespace Editor