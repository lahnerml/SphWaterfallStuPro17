#pragma once
#include "DomainHash.h"

namespace DomainHash {

	int DomainHash::hash(const Vector3& vector) {
		int x, y, z;
		x = static_cast<int>(vector.x);
		y = static_cast<int>(vector.y);
		z = static_cast<int>(vector.z);
		y = y << 10;
		z = z << 20;
		return (x + y + z);
	}

	Vector3 DomainHash::unhash(const int& unique_id) {
		int z = unique_id >> 20;
		int y = (unique_id - (z << 20)) >> 10;
		int x = unique_id - (z << 20) - (y << 10);
		return Vector3(x, y, z);
	}
}
