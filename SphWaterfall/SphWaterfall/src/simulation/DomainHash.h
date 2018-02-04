#pragma once
#include "../data/Vector3.h";

namespace DomainHash {
	int hash(const Vector3&);
	Vector3 unhash(const int&);
}