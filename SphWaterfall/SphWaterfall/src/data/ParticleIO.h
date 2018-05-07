#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <math.h>

#include "SphParticle.h"
#include "../visualization/util.h"

using namespace std;
class ParticleIO {
public:
	//Exportiert die Partikel als Datei
	static void exportParticles(unordered_map<int, vector<SphParticle>>& frames, string fileName);

	//Exportiert die Partikel im VTK-Format
	static void exportParticlesToVTK(vector<SphParticle>& particles, string fileName, int timestep, vector<int> proc_boundaries = {});

	//Importiert die Partikel aus Datei
	static vector<vector<SphParticle>> importParticles(string fileName);
};
