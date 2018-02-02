#pragma once

class Vector3 {
	public:
		double x, y, z;
		Vector3();
		Vector3(const double, const double, const double);
		~Vector3();

		friend Vector3 operator*(const Vector3&, const double);
		friend Vector3 operator*(const double, const Vector3&);

		friend Vector3 operator/(const Vector3&, const double);
		friend Vector3 operator%(const Vector3&, const double);
		friend Vector3 operator%(const Vector3&, const Vector3&);

		friend Vector3 operator+(const Vector3&, const Vector3&);
		friend Vector3 operator-(const Vector3&);
		friend Vector3 operator-(const Vector3&, const Vector3&);

		
		bool in_range_of(const Vector3&) const;
		double length() const;
		Vector3 normalize() const;
		Vector3 distanceTo(const Vector3&, const Vector3&);

	private:
};
