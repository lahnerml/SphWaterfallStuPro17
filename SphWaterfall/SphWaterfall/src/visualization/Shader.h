#pragma once
#include <iostream>
#include "Frame.h"

//Shader Klasse
class Shader {
public:
	//Führt einen Gausschen Weichzeichner angegeben oft durch, mit angegebener Matrix Größe
	static Frame applyGaussianSmoothing(Frame f, int shaderDepth, int matrixSize);
};