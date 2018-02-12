#pragma once
#include "Face.h"


Face::Face() :
	a(Vector3()),
	b(Vector3()),
	c(Vector3())
{
}

Face::Face(Vector3 a, Vector3 b, Vector3 c) :
	a(a), b(b), c(c)
{
}

void Face::MpiSendFace(Face face, int dest)
{
	double buf[9] =
	{
		face.a.x , face.a.y, face.a.z,
		face.b.x , face.b.y, face.b.z,
		face.c.x , face.c.y, face.c.z
	};

	MPI_Send(buf, 9, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
}

Face Face::MpiReceiveFace(int source)
{
	double buf[9];
	MPI_Recv(buf, 9, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
	return Face(Vector3(buf[0], buf[1], buf[2]),
		Vector3(buf[3], buf[4], buf[5]),
		Vector3(buf[6], buf[7], buf[8]));
}

std::ostream& operator<<(std::ostream &out, const Face& face)
{
	return out << "[" << face.a << ", " << face.b << ", " << face.c << "]";
}
