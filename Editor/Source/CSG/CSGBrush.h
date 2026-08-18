#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>
#include <Core/Shared/Uuid.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Core
{
	class Object;
	class Material;
}

namespace manifold
{
	class Manifold;
}

namespace Editor
{
	class CSGModel;

	class CSGBrush
	{
		friend class CSGModel;

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

		CSGBrush(CSGModel* parent);
		virtual ~CSGBrush();

		Core::Object* _object = nullptr;
		CSGModel* _parent = nullptr;
		bool _castShadows = true;
		BrushOperation _brushOperation = BrushOperation::Add;
		manifold::Manifold* _brushPtr = nullptr;
		uint32_t _originalId = 0;

		Core::List<glm::vec3> _vertices;
		Core::List<FaceInfo> _faces;

		Core::Uuid _uuid = Core::Uuid::create();

		void destroy();

	public:
		void rebuild();
		virtual void make() {};

		CSGModel* getParent() { return _parent; }

		manifold::Manifold* getBrushPtr() { return _brushPtr; }

		uint32_t getOriginalId() const { return _originalId; }

		BrushOperation getBrushOperation() { return _brushOperation; }
		void setBrushOperation(BrushOperation value);

		Core::Object* getObject() const { return _object; }

		Core::List<uint32_t> getFlatIndices();
		Core::List<glm::vec3>& getVertices() { return _vertices; }
		Core::List<FaceInfo>& getFaces() { return _faces; }

		bool getCastShadows() const { return _castShadows; }
		void setCastShadows(bool value) { _castShadows = value; }

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

		Core::Uuid getId() { return _uuid; }
	};

	CSGBrush::uv_t operator*(double s, const CSGBrush::uv_t & t);
	CSGBrush::uv_t& operator+=(CSGBrush::uv_t& t1, const CSGBrush::uv_t& t2);
}