#pragma once
#include "Vector3.h"
#include <math.h>

Vector3::Vector3() :
	x(0.0),
	y(0.0),
	z(0.0)
{

}

Vector3::Vector3(const double x, const double y, const double z) :
	x(x),
	y(y),
	z(z) 
{

}

Vector3::~Vector3() {

}

Vector3 operator*(const Vector3& vector, const double factor) {
	return Vector3(vector.x * factor, vector.y * factor, vector.z * factor);
}

Vector3 operator*(const double factor, const Vector3& vector) {
	return vector * factor;
}

Vector3 operator/(const Vector3& vector, const double factor) {
	return Vector3(vector.x / factor, vector.y / factor, vector.z / factor);
}

Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator-(const Vector3& a)
{
	return Vector3(-a.x, -a.y, -a.z);
}

Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

bool Vector3::all_dimensions_smaller_or_equal_then(const Vector3& b) const
{
	return x <= b.x && y <= b.y && z <= b.z;
}

double Vector3::length() const {
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalize() const {
	return *this / length();
}
