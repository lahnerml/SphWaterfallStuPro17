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

		friend Vector3 operator+(const Vector3&, const Vector3&);
		friend Vector3 operator-(const Vector3&);
		friend Vector3 operator-(const Vector3&, const Vector3&);

		
		bool all_dimensions_smaller_or_equal_then(const Vector3&) const;
		double length() const;
		Vector3 normalize() const;
	private:
};
