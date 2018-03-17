#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "SphParticle.h"
#include "../visualization/util.h"

using namespace std;
class ParticleIO {
public:
	static void exportParticles(unordered_map<int, vector<SphParticle>> frames, string fileName);
	static vector<vector<SphParticle>> importParticles(string fileName);
};