#pragma once

#include <Shared/String.h>
#include <Shared/List.h>
#include <Shared/Guid.h>
#include <Shared/Hash.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Core
{
	class Object;
	class Material;
	class Transform;
}

namespace carve
{
	namespace poly
	{
		class Polyhedron;
	}

	namespace interpolate
	{
		template<typename attr_t>
		class FaceVertexAttr;

		template<typename attr_t>
		class FaceAttr;
	}
}

namespace Editor
{
	class CSGModel;

	class CSGBrush
	{
	public:
		enum class BrushOperation
		{
			Add,
			Subtract
		};

		struct uv_t
		{
		public:
			float u = 0.0f;
			float v = 0.0f;

			uv_t() : u(0.0f), v(0.0f) { }
			uv_t(float _u, float _v) : u(_u), v(_v) { }
		};
	protected:
		struct FaceInfo
		{
			Core::List<uint32_t> indices;
			Core::List<glm::vec2> texCoords;
			glm::vec2 texCoordsScale = glm::vec2(1.0f);
			glm::vec2 texCoordsOffset = glm::vec2(0.0f);
			float texCoordsRotation = 0.0f;
			bool smoothNormals = true;

			Core::Material* material = nullptr;
		};

		UString name = "";
		CSGModel* parent = nullptr;
		bool castShadows = true;
		BrushOperation brushOperation = BrushOperation::Add;
		Core::Object* object = nullptr;
		Core::Transform* transform = nullptr;
		carve::poly::Polyhedron* brushPtr = nullptr;

		Core::List<glm::vec3> vertices;
		Core::List<FaceInfo> faces;

		UString guid = FromStdString(Core::Guid::genGuid());
		size_t hash = Core::Hash::getHash(ToStdString(guid).c_str());

		void destroy();

	public:
		CSGBrush();
		virtual ~CSGBrush();

		void rebuild();
		virtual void make() {};

		UString getName() { return name; }
		void setName(UString value) { name = value; }

		CSGModel* getParent() { return parent; }
		void setParent(CSGModel* value) { parent = value; }

		carve::poly::Polyhedron* getBrushPtr() { return brushPtr; }

		BrushOperation getBrushOperation() { return brushOperation; }
		void setBrushOperation(BrushOperation value);

		void bind
		(
			carve::interpolate::FaceVertexAttr<uv_t>* fv_uv,
			carve::interpolate::FaceAttr<Core::Material*>* f_material,
			carve::interpolate::FaceAttr<int>* f_layer,
			carve::interpolate::FaceAttr<bool>* f_castShadows,
			carve::interpolate::FaceAttr<bool>* f_smoothNormals,
			carve::interpolate::FaceAttr<size_t>* f_brushId
		);

		Core::Object* getObject() { return object; }
		Core::Transform* getTransform() { return transform; }

		Core::List<glm::vec3>& getVertices() { return vertices; }
		Core::List<FaceInfo>& getFaces() { return faces; }

		bool getCastShadows() { return castShadows; }
		void setCastShadows(bool value);

		Core::Material* getMaterial(int faceIndex);
		void setMaterial(int faceIndex, Core::Material* value);

		glm::vec2 getUV(int faceIndex, int vertIndex);
		void setUV(int faceIndex, int vertIndex, glm::vec2 value);

		glm::vec2 getUVScale(int faceIndex);
		void setUVScale(int faceIndex, glm::vec2 value);

		glm::vec2 getUVOffset(int faceIndex);
		void setUVOffset(int faceIndex, glm::vec2 value);

		float getUVRotation(int faceIndex);
		void setUVRotation(int faceIndex, float value);

		bool getSmoothNormals(int faceIndex);
		void setSmoothNormals(int faceIndex, bool value);
	};

	CSGBrush::uv_t operator*(double s, const CSGBrush::uv_t & t);
	CSGBrush::uv_t& operator+=(CSGBrush::uv_t& t1, const CSGBrush::uv_t& t2);
}