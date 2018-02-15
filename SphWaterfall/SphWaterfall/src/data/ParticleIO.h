#include "FluidParticle.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

static void exportParticles(unordered_map<int, vector<FluidParticle>> frames, string fileName) {
	ofstream file(fileName);
	if (file.is_open())
	{
		for (int i = 0; i < frames.size(); i++) {
			file << "F#" << i << "#" << frames.size() << "\n";

			for (int g = 0; g < frames.at(i).size(); g++) {
				file << frames.at(i).at(g).position.x << "#"
					<< frames.at(i).at(g).position.y << "#"
					<< frames.at(i).at(g).position.z << "#"
					<< frames.at(i).at(g).velocity.x << "#"
					<< frames.at(i).at(g).velocity.y << "#"
					<< frames.at(i).at(g).velocity.z << "#"
					<< frames.at(i).at(g).mass << "\n";
			}
		}
		file.close();
	}
	else cout << "Unable to open file";
}

static vector<vector<FluidParticle>> importParticles(string fileName) {
	vector<vector<FluidParticle>> frames;

	string line;
	ifstream myfile(fileName);
	if (myfile.is_open())
	{
		int counter = 0;
		vector<FluidParticle> frame;
		while (getline(myfile, line))
		{
			counter++;
			if (startsWith(line, "F") && !(frame.empty())) {
				frames.emplace_back(frame);
				frame.clear();
				continue;
			}

			vector<string> splittedLine = split(line, '#');

			if (splittedLine.size() != 7) {
				cout << "Malformed line found at " << counter << ". Skipping...\n";
				continue;
			}

			frame.emplace_back(FluidParticle(
				Vector3(
					stod(splittedLine.at(0), nullptr),
					stod(splittedLine.at(1), nullptr),
					stod(splittedLine.at(2), nullptr)
				),
				Vector3(
					stod(splittedLine.at(3), nullptr),
					stod(splittedLine.at(4), nullptr),
					stod(splittedLine.at(5), nullptr)
				),
				stod(splittedLine.at(6), nullptr)
			));
		}
		myfile.close();
	}

	else cout << "Unable to open file";

	return frames;
}