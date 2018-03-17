#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "TerrainParser.h"
#include "Terrain.h"
#include "../data/Vector3.h"

Terrain TerrainParser::loadFromFile(std::string fileName) {
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<int> faces;

	//Read 3d object data from file
	readObjFile(fileName, vertices, normals, faces);

	//Generate terrain
	return Terrain::Terrain(vertices, normals, faces);
}

void TerrainParser::readObjFile(std::string fileName, std::vector<Vector3> &vertices, std::vector<Vector3> &normals, std::vector<int> &faces)
{
	std::ifstream in(fileName);
	if (!in)
	{
		//cerr << "Cannot open " << filename << endl; exit(1);
	}

	std::string line;
	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			// Read vertex from file
			std::istringstream s(line.substr(2));
			Vector3 v;
			s >> v.x; s >> v.y; s >> v.z;
			vertices.push_back(v);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			//Read vertex normal from file
			std::istringstream s(line.substr(3));
			Vector3 n;
			s >> n.x; s >> n.y; s >> n.z;
			normals.push_back(n);
		}
		else if (line.substr(0, 2) == "f ")
		{
			//Read face from file
			std::istringstream s(line.substr(2));
			int a, b, c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			
			faces.push_back(a); faces.push_back(b); faces.push_back(c);
		}
		else if (line[0] == '#')
		{
			// Ignore comments
		}
		else
		{
			//Ignore other lines
		}
	}
}