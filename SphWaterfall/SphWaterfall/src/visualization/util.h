#pragma once

#include "../data/Vector3.h"
#include "../data/FluidParticle.h"
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;


static Frame applySmoothingShader(Frame f, int shaderDepth) {
	Frame newFrame;
	for (int k = 0; k < shaderDepth; k++) {
		newFrame = Frame(f.getWidth(), f.getHeight(), f.getFrameID());

		for (int x = 0; x < f.getWidth(); x++) {
			for (int y = 0; y < f.getHeight(); y++) {
				if (f.getPixel(x, y).usesShader()) {
					unsigned short r = 0;
					unsigned short g = 0;
					unsigned short b = 0;

					//std::cout << "ORG: R." << f.getPixel(x, y).getRedValue() << " G." << f.getPixel(x, y).getGreenValue() << " B." << f.getPixel(x, y).getBlueValue() << "\n";

					unsigned short divider = 0;
					for (int i = -6; i <= 6; i++) {
						for (int j = -6; j <= 6; j++) {
							if (x + i < 0 || x + i > f.getWidth() || y + j < 0 || y + j > f.getHeight()) continue;

							if (!f.getPixel(x + i, y + j).usesShader()) continue;

							//		std::cout << "ORG: R." << f.getPixel(x+i, y+j).getRedValue() << " G." << f.getPixel(x + i, y + j).getGreenValue() << " B." << f.getPixel(x + i, y + j).getBlueValue() << "\n";
							r += f.getPixel(x + i, y + j).getRedValue();
							g += f.getPixel(x + i, y + j).getGreenValue();
							b += f.getPixel(x + i, y + j).getBlueValue();
							divider++;
						}
					}
					//std::cout << "NEW: R." << r << " G." << g << " B." << b << "\n";
					Pixel p = Pixel(r / divider, g / divider, b / divider);
					p.setShaderUsage(true);
					newFrame.setPixel(x, y, p);
				}
				else {
					newFrame.setPixel(x, y, f.getPixel(x, y));
				}
			}
		}
		f = newFrame;
	}

	return newFrame;
}

static Vector3 findRightSkalar(Vector3 vec, Vector3 right) {
	Vector3 left = Vector3(right.x * -1, right.y * -1, right.z * -1);

	if (vec.x >= 0) {
		if ((vec.z > 0 && right.x < 0) || (vec.z < 0 && right.x > 0) || (vec.z == 0 && right.z > 0)) return left;
	}
	else {
		if ((vec.z > 0 && right.x < 0) || (vec.z < 0 && right.x > 0) || (vec.z == 0 && right.z < 0)) return left;
	}

	return right;
}

static Vector3 findSkalarVectorWithYZero(Vector3 vector) {
	double x = 1;
	double y = 0;
	double z = vector.z == 0 ? 0.f : vector.x / (-1 * vector.z);

	return findRightSkalar(vector, Vector3(x, y, z));
}

static Vector3 findUpVector(Vector3 first, Vector3 second) {
	Vector3 up = Vector3(first.y * second.z - first.z * second.y,
		first.z * second.x - first.x * second.z,
		first.x * second.y - first.y * second.x);

	if (up.y > 0) return up;
	return up*-1;
}

static std::vector<DebugObject> convertSphParticles(std::vector<FluidParticle> &particles) {
	std::vector<DebugObject> output;
	for (unsigned int i = 0; i < particles.size(); i++) {
		output.emplace_back(DebugObject(particles[i].position, 0.4f));
	}

	return output;
}

static std::vector<ParticleObject> convertFluidParticles(std::vector<FluidParticle> &particles) {
	std::vector<ParticleObject> output;
	for (unsigned int i = 0; i < particles.size(); i++) {
		output.emplace_back(ParticleObject(particles.at(i).position, particles.at(i).mass / (double) 5));
	}

	return output;
}

static void writeFrameToBitmap(Frame f, const char* fileName, unsigned int w, unsigned int h) {
	FILE *file;
	unsigned char *img = NULL;
	int filesize = 54 + 3 * w*h;

	img = (unsigned char *)malloc(3 * w*h);
	memset(img, 0, 3 * w*h);

	for (int i = 0; i<w; i++)
	{
		for (int j = 0; j<h; j++)
		{
			int x = i; int y = (h - 1) - j;
			int r = f.getPixel(x, y).getRedValue();
			int g = f.getPixel(x, y).getGreenValue();
			int b = f.getPixel(x, y).getBlueValue();
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			img[(x + y * w) * 3 + 2] = (unsigned char)(r);
			img[(x + y * w) * 3 + 1] = (unsigned char)(g);
			img[(x + y * w) * 3 + 0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	file = fopen(fileName, "wb");
	fwrite(bmpfileheader, 1, 14, file);
	fwrite(bmpinfoheader, 1, 40, file);
	for (int i = 0; i<h; i++)
	{
		fwrite(img + ((h-i-1)*w*bytesPerPixel), 3, w, file);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, file);
	}

	free(img);
	fclose(file);
}

static vector<string> split(const string &s, const char &c)
{
	string buff{ "" };
	vector<string> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != "") v.push_back(buff);

	return v;
}

static bool startsWith(const string &s, const string &sequence) {
	if (s.size() < sequence.size()) return false;

	for (int i = 0; i < sequence.size(); i++) {
		if (s.at(i) != sequence.at(i)) return false;
	}

	return true;
}