#pragma once
#include "mpi.h"

#include "../data/Vector3.h"

class Face {
public:
	Face();
	Face(Vector3, Vector3, Vector3);

	//TODO make const
	Vector3 a;
	Vector3 b;
	Vector3 c;

	static void MpiSendFace(Face face, int dest);
	static Face MpiReceiveFace(int source);

	friend std::ostream& operator<<(std::ostream &out, const Face&);
};