#include "Terrain.h"


Terrain::Terrain(std::vector<Vector3> &vertices, std::vector<int> &faces) :
	vertices(vertices), nullFace(Face())
{
	for(int i = 0; i < faces.size(); i++)
	{
		//Param faces contains only index of face's vertices
		this->faces.push_back(Face(vertices[faces[i]], vertices[faces[i + 1]], vertices[faces[i + 2]]));
		i += 2;
	}
}

Terrain::Terrain() :
	vertices(std::vector<Vector3>()),
	faces(std::vector<Face>()),
	nullFace(Face())
{
}


const size_t Terrain::getVertexCount()
{
	return this->vertices.size();
}

const size_t Terrain::getFaceCount()
{
	return this->faces.size();
}


const Vector3 Terrain::getVertex(int index)
{
	if (index < 0 || index >= this->vertices.size())
		return Vector3();

	return this->vertices[index];
}

const Face& Terrain::getFace(int faceId)
{
	if (faceId < 0 || faceId >= this->faces.size())
		return nullFace;

	return this->faces[faceId];
}
