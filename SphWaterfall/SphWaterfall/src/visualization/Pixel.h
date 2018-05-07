#pragma once

#include <limits>

//Pixel Klasse; enthält RGB-Farbwerte
class Pixel {
public:
	Pixel();
	Pixel(unsigned short red, unsigned short green, unsigned short blue);

	unsigned short getRedValue();
	unsigned short getGreenValue();
	unsigned short getBlueValue();

	void setColor(unsigned short red, unsigned short green, unsigned short blue);
	void setRed(unsigned short red);
	void setGreen(unsigned short green);
	void setBlue(unsigned short blue);

	//Shader Funktionalitaet; Kontrolliert ob der Pixel im Shader miteinbezogen wird
	bool usesShader();
	void setShaderUsage(bool shaderUsage);

	//Setzt die Geometrie Tiefe für den Pixel; Wird bentuzt um das zu unterscheiden ob WasserPartikel hinter oder vor dem Terrain liegen
	void setBaseDepth(double baseDepth);
	double getBaseDepth();

private:
	unsigned short red;
	unsigned short green;
	unsigned short blue;
	double baseDepth;
	bool shaderUsage;
};