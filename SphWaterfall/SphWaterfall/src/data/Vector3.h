#pragma once

class Vector3 {
	public:
		double x, y, z;
		Vector3();
		Vector3(double, double, double);
		~Vector3();

		friend Vector3 operator*(const Vector3&, double);
		friend Vector3 operator*(double, const Vector3&);
		friend Vector3 operator/(const Vector3&, double);

		friend Vector3 operator+(const Vector3&, const Vector3&);

		double length() const;
		Vector3 normalize() const;
	private:
};
