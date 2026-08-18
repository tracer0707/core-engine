#include "CSGBrushCube.h"

namespace Editor
{
	CSGBrushCube::CSGBrushCube(CSGModel* parent) : CSGBrush(parent)
	{
		make();
	}

	CSGBrushCube::~CSGBrushCube()
	{
		_vertices.clear();
		_faces.clear();
	}

	void CSGBrushCube::make()
	{
		_vertices.clear();
		_faces.clear();

		_vertices.add(glm::vec3(+0.5f, +0.5f, +0.5f));
		_vertices.add(glm::vec3(-0.5f, +0.5f, +0.5f));
		_vertices.add(glm::vec3(-0.5f, -0.5f, +0.5f));
		_vertices.add(glm::vec3(+0.5f, -0.5f, +0.5f));
		_vertices.add(glm::vec3(+0.5f, +0.5f, -0.5f));
		_vertices.add(glm::vec3(-0.5f, +0.5f, -0.5f));
		_vertices.add(glm::vec3(-0.5f, -0.5f, -0.5f));
		_vertices.add(glm::vec3(+0.5f, -0.5f, -0.5f));

		FaceInfo face1, face2, face3, face4, face5, face6;

		face1.indices = { 0, 1, 2, 3 };
		face2.indices = { 7, 6, 5, 4 };
		face3.indices = { 0, 4, 5, 1 };
		face4.indices = { 2, 6, 7, 3 };
		face5.indices = { 3, 7, 4, 0 };
		face6.indices = { 1, 5, 6, 2 };

		face1.texCoords = { {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f} };
		face2.texCoords = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		face3.texCoords = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		face4.texCoords = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		face5.texCoords = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		face6.texCoords = { {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f} };

		_faces.add(face1); //+Z
		_faces.add(face2); //-Z
		_faces.add(face3); //+Y
		_faces.add(face4); //-Y
		_faces.add(face5); //+X
		_faces.add(face6); //-X
	}
}