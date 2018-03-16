#pragma once
#include "mpi.h"

#include "../data/Vector3.h"

class Face {
public:
	Face();
	Face(Vector3, Vector3, Vector3);

	const Vector3 a;
	const Vector3 b;
	const Vector3 c;

	static void MpiSendFace(Face face, int dest);
	static Face MpiReceiveFace(int source);

	Face(const Face&) = default;
	Face& operator=(const Face&) = default;

	friend std::ostream& operator<<(std::ostream &out, const Face&);
};