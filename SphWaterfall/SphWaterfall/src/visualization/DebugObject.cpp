#pragma once
#include "DebugObject.h"
#include "Pixel.h"

DebugObject::DebugObject() {
	this->radius = radius;
	this->location = location;
}

DebugObject::DebugObject(Vector3 location, double radius) {
	this->radius = radius;
	this->location = location;
}

Vector3 DebugObject::getLocation() {
	return this->location;
}

double DebugObject::getRadius() {
	return this->radius;
}

Pixel DebugObject::getColor() {
	return Pixel(0, 255, 0);
}

bool DebugObject::intersects(Ray &ray, double &distance) {
	distance = std::numeric_limits<double>::max();

	Vector3 temp = Vector3(ray.origin.x - this->location.x, ray.origin.y - this->location.y, ray.origin.z - this->location.z);

	double a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y + ray.direction.z * ray.direction.z;
	double b = 2 * (ray.direction.x * temp.x + ray.direction.y * temp.y + ray.direction.z * temp.z);
	double c = (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z) - (this->radius*this->radius);

	if ((b*b) - 4 * a*c == 0) {
		distance = (-1 * b) / (2 * a);
	}
	else if ((b*b) - 4 * a*c > 0) {
		float t1 = (-1 * b + sqrt((b*b) - 4 * a*c)) / (2 * a);
		float t2 = (-1 * b - sqrt((b*b) - 4 * a*c)) / (2 * a);

		distance = t1 < t2 ? t1 : t2;
	}

	return ((b*b) - 4 * a*c) < 0 ? false : true;
}
