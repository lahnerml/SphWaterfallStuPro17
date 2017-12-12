#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "ITerrainParser.h"
#include "../data/Vector3.h"

void TerrainParser::loadFromFile(std::string fileName) {
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<short> faces;

	//Read 3d object data from file
	readObjFile(fileName, vertices, normals, faces);

	//Generate terrain

}

void readObjFile(std::string filename, std::vector<Vector3> &vertices, std::vector<Vector3> &normals, std::vector<short> &faces)
{
	ifstream in(filename, ios::in);
	if (!in)
	{
		cerr << "Cannot open " << filename << endl; exit(1);
	}

	string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s(line.substr(2));
			Vector3 v;
			s >> v.x; s >> v.y; s >> v.z;
			vertices.push_back(v);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			std::istringstream s(line.substr(3));
			Vector3 n;
			s >> n.x; s >> n.y; s >> n.z;
			normals.push_back(n);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream s(line.substr(2));
			short a, b, c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			elements.push_back(a); elements.push_back(b); elements.push_back(c);
		}
		else if (line[0] == '#')
		{
			/* ignoring this line */
		}
		else
		{
			/* ignoring this line */
		}
	}
}