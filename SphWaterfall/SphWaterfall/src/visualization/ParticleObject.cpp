#include "ParticleObject.h"

ParticleObject::ParticleObject() {
	this->radius = radius;
	this->location = location;
}

ParticleObject::ParticleObject(Vector3 location, double radius) {
	this->radius = radius;
	this->location = location;
}

Vector3 ParticleObject::getLocation() {
	return this->location;
}

double ParticleObject::getRadius() {
	return this->radius;
}

bool ParticleObject::intersects(Ray &ray, double &distance, double &waterDepth, double maxDepth) {
	distance = std::numeric_limits<double>::max();

	Vector3	temp = ray.origin - this->location;

	double a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y + ray.direction.z * ray.direction.z;
	double b = 2 * (ray.direction.x * temp.x + ray.direction.y * temp.y + ray.direction.z * temp.z);
	double c = (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z) - (this->radius*this->radius);

	if ((b*b) - 4 * a*c == 0) {
		distance = (-1 * b) / (2 * a);
	}
	else if ((b*b) - 4 * a*c > 0) {
		float t1 = (-1 * b + sqrt((b*b) - 4 * a*c)) / (2 * a);
		float t2 = (-1 * b - sqrt((b*b) - 4 * a*c)) / (2 * a);

		waterDepth += t1 > t2 ? (t1 > maxDepth ? maxDepth : t1) - (t2 > maxDepth ? 0 : t2) : (t2 > maxDepth ? maxDepth : t2) - (t1 > maxDepth ? 0 : t1);

		distance = t1 < t2 ? t1 : t2;
	}

	return ((b*b) - 4 * a*c) < 0 ? false : true;
}

void ParticleObject::MpiSendPObject(ParticleObject pObj, int dest)
{
	double buf[4] =
	{
		pObj.getLocation().x, pObj.getLocation().y, pObj.getLocation().z, pObj.getRadius()
	};

	MPI_Send(buf, 4, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
}

ParticleObject ParticleObject::MpiReceivePObject(int source)
{
	double buf[4];
	MPI_Recv(buf, 4, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	return ParticleObject(Vector3(buf[0], buf[1], buf[2]), buf[3]);
}
