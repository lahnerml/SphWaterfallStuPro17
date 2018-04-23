#define _USE_MATH_DEFINES
#include "Camera.h"

Camera::Camera() {
	this->width = 0;
	this->height = 0;

	this->location = Vector3(0, 0, 0);
	this->direction = Vector3(0, 0, 0);

	this->switchFrameID = std::numeric_limits<unsigned int>::max();

	baseFrameOpen = Frame(this->width, this->height);
	baseFrameClosed = Frame(this->width, this->height);
}

Camera::Camera(Vector3 location, Vector3 direction, unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	
	this->location = location;
	this->direction = direction;

	this->switchFrameID = std::numeric_limits<unsigned int>::max();

	baseFrameOpen = Frame(this->width, this->height);
	baseFrameClosed = Frame(this->width, this->height);
}

void Camera::debugRenderFrame(std::vector<DebugObject> particles) {

	Frame frame = Frame(this->width, this->height);
	const double aspectRatio = (double)width / (double)height;
	// view plane parameters
	const double l = -1.f *aspectRatio;   //left
	const double r = +1.f *aspectRatio;   //right
	const double b = -1.f;				  // bottom
	const double t = +1.f;				  // top
	const double d = +2.f;				  // distance to camera


	Vector3 vec_u = findSkalarVectorWithYZero(this->direction).normalize();
	Vector3 vec_v = findUpVector(this->direction, vec_u).normalize();

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			double u = l + (r - l) * (x + 0.5f) / this->width;
			double v = t + (b - t) * (y + 0.5f) / this->height;

			Vector3 vec_s = vec_u * u + vec_v * v + this->direction * d;

			Ray ray = Ray(vec_s.normalize(), this->location);


			frame.setPixel(x, y, castDebugRay(ray, particles));
		}
	}

	this->outputDebugFrame(frame, "debug_output.bmp");
}

Pixel Camera::castDebugRay(Ray ray, std::vector<DebugObject> particles) {
	double bestDistance = std::numeric_limits<float>::max();
	double waterDepth = 0;
	DebugObject hitObject;
	DebugObject *hit = &hitObject;
	hit = nullptr;

	//Calculate hit for every object
	for (int i = 0; i < particles.size(); i++) {
		DebugObject &obj = particles.at(i);
		double currDist = bestDistance;

		if (obj.intersects(ray, currDist, waterDepth)) {
			if (currDist < bestDistance) {
				bestDistance = currDist;
				hit = &obj;
			}
		}
	}

	return hit == nullptr ? Pixel(0, 0, 0) : Pixel(0, 255, 0);
}

Pixel Camera::castVolumeRay(Ray ray, std::vector<ParticleObject> particles, Pixel basePixel) {
	double bestDistance = std::numeric_limits<float>::max();
	double waterDepth = 0;
	ParticleObject hitObject;
	ParticleObject *hit = &hitObject;
	hit = nullptr;

	//Check for collisions with Particles and keep track off the nearest and furthest hit
	for (int i = 0; i < particles.size(); i++) {
		ParticleObject &obj = particles.at(i);
		double currDist = bestDistance;

		if (obj.intersects(ray, currDist, waterDepth, basePixel.getBaseDepth())) {
			if (currDist < bestDistance) {
				bestDistance = currDist;
				hit = &obj;
			}
		}
	}

	waterDepth = waterDepth > 15 ? 15 : waterDepth; //Cap waterDepth at 15

	//Copy the pixel from the base Frame
	Pixel pixel = Pixel(basePixel.getRedValue(), basePixel.getGreenValue(), basePixel.getRedValue());

	if (waterDepth <= 0) {
		return pixel;
	}

	Pixel waterColor = Pixel(30, 30, 170); //Color of "pure water"
	pixel.setShaderUsage(true);
	//Shade the pixel depending on water depth
	if (hit != nullptr) {
	//New Shading, should work smiliar to alpha
		int diffR = waterColor.getRedValue() - pixel.getRedValue();
		int diffG = waterColor.getGreenValue() - pixel.getGreenValue();
		int diffB = waterColor.getBlueValue() - pixel.getBlueValue();

		double percentage = 1.2 - (exp(-0.1*waterDepth));//(double)waterDepth / (double)15;

		double newR = (double)pixel.getRedValue() + (double) diffR * percentage;
		double newG = (double)pixel.getGreenValue() + (double)diffG * percentage;
		double newB = (double)pixel.getBlueValue() + (double)diffB * percentage;

		pixel.setRed((unsigned short)newR);
		pixel.setGreen((unsigned short)newG);
		pixel.setBlue((unsigned short)newB);
	}

	return pixel;
}

Frame Camera::renderFrame(std::vector<ParticleObject> particles, int frameID) {
	Frame frame = Frame(this->width, this->height);

	// view plane parameters
	const double aspectRatio = (double)width / (double)height;
	const double l = -1.f * aspectRatio;   //left
	const double r = +1.f * aspectRatio;   //right
	const double b = -1.f;				  // bottom
	const double t = +1.f;				  // top
	const double d = +2.f;				  // distance to camera

	//Find the skalar vectors to the view direction
	Vector3 vec_u = findSkalarVectorWithYZero(this->direction).normalize();
	Vector3 vec_v = findUpVector(this->direction, vec_u).normalize();

	//Cast ray for every pixel
	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			double u = l + (r - l) * (x + 0.5f) / this->width;
			double v = t + (b - t) * (y + 0.5f) / this->height;

			//Ray Direction Vector
			Vector3 vec_s = vec_u * u + vec_v * v + this->direction * d;
			Ray ray = Ray(vec_s, this->location);

			//Set pixel in Frame
			frame.setPixel(x, y, castVolumeRay(ray, particles, getCurrentlyUsedBaseFrame(frameID).getPixel(x, y)));
			
		}
	}

	return Shader::applyGaussianSmoothing(frame, 5, 8);
}

void Camera::renderGeometryFrames(Terrain terrain, Terrain gate) {
	const double aspectRatio = (double)width / (double)height;
	const double l = -1.f *aspectRatio;
	const double r = +1.f *aspectRatio;
	const double b = -1.f;
	const double t = +1.f;
	const double d = +2.f;

	Vector3 vec_u = findSkalarVectorWithYZero(this->direction).normalize();
	Vector3 vec_v = findUpVector(this->direction, vec_u).normalize();


	//Build frame for Terrain with open Gate ############################

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			double u = l + (r - l) * (x + 0.5f) / this->width;
			double v = t + (b - t) * (y + 0.5f) / this->height;

			Vector3 vec_s = vec_u * u + vec_v * v + this->direction * d;
			Ray ray = Ray(vec_s, this->location);

			baseFrameOpen.setPixel(x, y, castTerrainRay(ray, terrain));
		}
	}

	//Build Frame for Terrain with closed Gate ###########################

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			double u = l + (r - l) * (x + 0.5f) / this->width;
			double v = t + (b - t) * (y + 0.5f) / this->height;

			Vector3 vec_s = vec_u * u + vec_v * v + this->direction * d;
			Ray ray = Ray(vec_s, this->location);

			baseFrameClosed.setPixel(x, y, castTerrainGateRay(ray, terrain, gate));
		}
	}

	//#####################################################################
	writeFrameToBitmap(baseFrameOpen, "terrain_debug_open.bmp", baseFrameOpen.getWidth(), baseFrameOpen.getHeight());
	writeFrameToBitmap(baseFrameClosed, "terrain_debug_closed.bmp", baseFrameClosed.getWidth(), baseFrameClosed.getHeight());
}

Pixel Camera::castTerrainRay(Ray ray, Terrain& terrain) {
	double bestDistance = std::numeric_limits<float>::max();
	int hitIndex = -1;

	Pixel initColor = Pixel(200, 200, 200); //Make the background gray

	for (int i = 0; i < terrain.getFaceCount(); i++) {
		double currDist = bestDistance;

		if (intersectsWithFace(ray, terrain.getFace(i), currDist)) {
			if (currDist < bestDistance) {
				bestDistance = currDist;
				hitIndex = i;
			}
		}
	}

	initColor.setBaseDepth(bestDistance);

	//Calculate Light to not have just brown everywhere
	if (hitIndex >= 0){

		Vector3 planar1 = terrain.getFace(hitIndex).a - terrain.getFace(hitIndex).b;
		Vector3 planar2 = terrain.getFace(hitIndex).a - terrain.getFace(hitIndex).c;

		//Calculate norm vector
		Vector3 n = planar1.cross(planar2).normalize();

		Vector3 staticLight = Vector3(0, 200, 0);
		Vector3 lightToHit = (staticLight - (ray.origin + ray.direction * bestDistance)).normalize();

		//Calculate angle between norm vector of the plane and vector between light and hit point
		double rad = acos(n.dot(lightToHit) / (n.length() * lightToHit.length()));

		//Check if the angle is more than 180°, if so substract 180°
		rad = rad > M_PI ? rad - M_PI : rad;

		//Get a factor between 1 and 0; 90° = 0 ; 0° = 180° = 1
		double factor = 1 - (rad / (M_PI / 2));

		//Darken the pixel based on the factor
		Pixel p = Pixel(127 * factor, 67 * factor, 67 * factor);

		return p;
    }

	return initColor;
}

Pixel Camera::castTerrainGateRay(Ray ray, Terrain& terrain, Terrain& gate) {
	double bestDistance = std::numeric_limits<float>::max();
	int hitIndex = -1;

	Pixel initColor = Pixel(200, 200, 200); //Make the background gray

	for (int i = 0; i < terrain.getFaceCount() + gate.getFaceCount(); i++) {
		double currDist = bestDistance;

		if (i >= terrain.getFaceCount()) {
			if (intersectsWithFace(ray, gate.getFace(i - terrain.getFaceCount()), currDist)) {
				if (currDist < bestDistance) {
					bestDistance = currDist;
					hitIndex = i;
				}
			}
		}
		else {
			if (intersectsWithFace(ray, terrain.getFace(i), currDist)) {
				if (currDist < bestDistance) {
					bestDistance = currDist;
					hitIndex = i;
				}
			}
		}
	}

	initColor.setBaseDepth(bestDistance);

	//Calculate Light to not have just brown everywhere
	if (hitIndex >= 0) {

		Vector3 planar1 = terrain.getFace(hitIndex).a - terrain.getFace(hitIndex).b;
		Vector3 planar2 = terrain.getFace(hitIndex).a - terrain.getFace(hitIndex).c;

		//Calculate norm vector
		Vector3 n = planar1.cross(planar2).normalize();

		Vector3 staticLight = Vector3(0, 200, 0);
		Vector3 lightToHit = (staticLight - (ray.origin + ray.direction * bestDistance)).normalize();

		//Calculate angle between norm vector of the plane and vector between light and hit point
		double rad = acos(n.dot(lightToHit) / (n.length() * lightToHit.length()));

		//Check if the angle is more than 180°, if so substract 180°
		rad = rad > M_PI ? rad - M_PI : rad;

		//Get a factor between 1 and 0; 90° = 0 ; 0° = 180° = 1
		double factor = 1 - (rad / (M_PI / 2));

		//Darken the pixel based on the factor
		Pixel p = Pixel(127 * factor, 67 * factor, 67 * factor);

		return p;
	}

	return initColor;
}

Frame& Camera::getCurrentlyUsedBaseFrame(unsigned int frameID) {
	return frameID >= switchFrameID ? baseFrameOpen : baseFrameClosed;
}

void Camera::setGateSwitchFrame(unsigned int frameID) {
	this->switchFrameID = frameID;
}

void Camera::outputDebugFrame(Frame f, const char* fileName) {
	writeFrameToBitmap(f, fileName, this->width, this->height);
}

Vector3 Camera::getLocation() {
	return this->location;
}

Vector3 Camera::getDirection() {
	return this->direction;
}
