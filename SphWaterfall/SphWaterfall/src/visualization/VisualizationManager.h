#pragma once

#include "Camera.h"
#include "util.h"
#include "../data/SphParticle.h"
#include "../data/ParticleIO.h"
#include <string>


class VisualizationManager {
public:
	//Initialisiert den VisualizationManager; erstellt eine Kamera 
	static void init(Vector3 cameraLocation, unsigned int frameWidth, unsigned int frameHeight, int rank);

	//Importiert das Terrain unf lässt es auf der Kamera rendern
	static void importTerrain(Terrain t, bool isGate);

	//Setzt den Frame ab welchem die offene Geometrie verwendet werden soll
	static void setSwitchFrame(int switch_frame);

	//Rendert Frames als Debug
	static void debugRenderFrame(std::vector<SphParticle> particles, string fileName);

	//Rendert Frames
	static void renderFrames(string inputFileName);

	//Rendert Frames mit MPI
	static void renderFramesDistributed(string inputFileName, int rank);

	//Generiert Test Frames mit zufaelligen Partikeln
	static void generateFrames(int frameCount, int particleCount);

private:
	static vector<SphParticle> generateDebugParticles(int count);

	static bool initilaized;

	static Camera camera;
	static Terrain terrain;
	static Terrain gate;
	static int switch_frame;
};
