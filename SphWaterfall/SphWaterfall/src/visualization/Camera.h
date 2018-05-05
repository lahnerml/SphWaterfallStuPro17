#pragma once
#include "Frame.h"
#include "DebugObject.h"
#include "Shader.h"
#include "../data/SphParticle.h"
#include "../data/Vector3.h"
#include "ParticleObject.h"
#include <vector>
#include <string>
#include "../geometry/Terrain.h"
#include "util.h"
#include "Ray.h"
#include <math.h>

class Camera {
public:
	Camera();
	Camera(Vector3 location, Vector3 direction, unsigned int width, unsigned int height, int switch_frame);

	//Rendert einen Frame basierend auf DebugObject
	void debugRenderFrame(std::vector<DebugObject> particles);

	//Rendert einen Frame basierend auf ParticleObject
	Frame renderFrame(std::vector<ParticleObject> particles, int frameID);

	//Rendert die Geometrie als Basis Frames für weitere Operationen
	void renderGeometryFrames(Terrain terrain, Terrain gate);

	//Schreibt einen Frame als bmp Datei
	void outputDebugFrame(Frame f, const char* fileName);

	//Definiert den Frame ab wann das offene Terrain als Basis Frame genommen wird
	void setGateSwitchFrame(unsigned int frameID);

	//Verteilt die Basis Frames auf alle Prozessoren
	void shareBaseFrame(int);

	Vector3 getLocation();
	Vector3 getDirection();

private:

	//Rendert einen Pixel basierend auf DebugObject
	Pixel castDebugRay(Ray& ray, std::vector<DebugObject> particles);
	//Rendert ein einzelnes Terrain
	Pixel castTerrainRay(Ray& ray, Terrain& terrain);
	//Rendert zwei Terrain Modelle in einen Frame; wird für das Gatter genutzt
	Pixel castTerrainGateRay(Ray& ray, Terrain& terrain, Terrain& gate);
	//Rendert ParticleObjects; arbeitet mit Wassertiefe
	Pixel castVolumeRay(Ray& ray, std::vector<ParticleObject> particles, Pixel basePixel);

	//Gibt einen Verweiß auf den aktuell genutzten Basis Frame zurueck
	Frame& getCurrentlyUsedBaseFrame(unsigned int frameID);

	Frame baseFrameOpen;
	Frame baseFrameClosed;
	Vector3 location;
	Vector3 direction;

	unsigned int width;
	unsigned int height;
	unsigned int switchFrameID;

};