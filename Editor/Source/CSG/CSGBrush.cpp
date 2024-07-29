#include "CSGBrush.h"

#include <carve/carve.hpp>
#include <carve/csg.hpp>
#include <carve/input.hpp>
#include <carve/interpolator.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <Math/Mathf.h>
#include <Components/Transform.h>

namespace Editor
{
	CSGBrush::CSGBrush()
	{
		transform = new Core::Transform();
		
		make();
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
			delete brushPtr;

		brushPtr = nullptr;
	}

	void CSGBrush::rebuild()
	{
		destroy();

		carve::input::PolyhedronData data;

		glm::mat4x4 mtx = glm::identity<glm::mat4x4>();

		mtx = transform->getTransformMatrix();

		for (auto& v : vertices)
		{
			glm::vec4 lp = glm::vec4(v, 1.0f);
			glm::vec3 p = mtx * lp;

			data.addVertex(carve::geom::VECTOR(p.x, p.y, p.z));
		}

		for (int i = 0; i < faces.count(); ++i)
		{
			FaceInfo& face = faces.get(i);

			if (face.indices.count() == 3)
				data.addFace(face.indices.get(0), face.indices.get(1), face.indices.get(2));
			else if (face.indices.count() == 4)
				data.addFace(face.indices.get(0), face.indices.get(1), face.indices.get(2), face.indices.get(3));
		}

		brushPtr = data.create();
	}

	void CSGBrush::setBrushOperation(BrushOperation value)
	{
		brushOperation = value;
		rebuild();
	}

	void CSGBrush::bind(carve::interpolate::FaceVertexAttr<uv_t>* fv_uv,
		carve::interpolate::FaceAttr<Core::Material*>* f_material,
		carve::interpolate::FaceAttr<int>* f_layer,
		carve::interpolate::FaceAttr<bool>* f_castShadows,
		carve::interpolate::FaceAttr<bool>* f_smoothNormals,
		carve::interpolate::FaceAttr<size_t>* f_brushId)
	{
		for (int i = 0; i < faces.count(); ++i)
		{
			FaceInfo& face = faces.get(i);

			for (int j = 0; j < face.texCoords.count(); ++j)
			{
				glm::vec2 uv = face.texCoords.get(j);

				uv.x = uv.x * face.texCoordsScale.x + face.texCoordsOffset.x;
				uv.y = uv.y * face.texCoordsScale.y + face.texCoordsOffset.y;

				uv = Core::Mathf::rotateUV(uv, Core::Mathf::fDeg2Rad * face.texCoordsRotation);

				fv_uv->setAttribute(&brushPtr->faces[i], j, uv_t(uv.x, uv.y));
			}

			f_material->setAttribute(&brushPtr->faces[i], faces.get(i).material);
			f_layer->setAttribute(&brushPtr->faces[i], 0);
			f_castShadows->setAttribute(&brushPtr->faces[i], castShadows);
			f_smoothNormals->setAttribute(&brushPtr->faces[i], faces.get(i).smoothNormals);
			f_brushId->setAttribute(&brushPtr->faces[i], hash);
		}
	}

	void CSGBrush::setCastShadows(bool value)
	{
		castShadows = value;
		rebuild();
	}

	Core::Material* CSGBrush::getMaterial(int faceIndex)
	{
		if (faceIndex < faces.count())
			return faces.get(faceIndex).material;

		return nullptr;
	}

	void CSGBrush::setMaterial(int faceIndex, Core::Material* value)
	{
		if (faceIndex < faces.count())
			faces.get(faceIndex).material = value;
	}

	glm::vec2 CSGBrush::getUV(int faceIndex, int vertIndex)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);

			if (vertIndex < face.texCoords.count())
				return face.texCoords.get(vertIndex);
		}

		return glm::vec2(0.0f);
	}

	void CSGBrush::setUV(int faceIndex, int vertIndex, glm::vec2 value)
	{
		if (faceIndex < faces.count())
		{
			FaceInfo& face = faces.get(faceIndex);

			if (vertIndex < face.texCoords.count())
				face.texCoords.get(vertIndex) = value;
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
}