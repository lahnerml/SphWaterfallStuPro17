#include "TerrainParser.h"

Terrain TerrainParser::loadFromFile(std::string fileName) {
	std::vector<Vector3> vertices;
	std::vector<int> faces;
	std::string line;

	//Read 3d object data from file
	std::ifstream in(fileName);
	if (!in)
	{
		std::cerr << "Cannot open \"" << fileName << "\"" << std::endl;
	}

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

	//Generate terrain
	return Terrain(vertices, faces);
}