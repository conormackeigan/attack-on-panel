#pragma once
#pragma warning(disable:4244) // double to T data loss from <math> functions
#pragma warning(disable:4146) // unary minus operator applied to unsigned type

/**
	Purpose: Template container for 2D vectors and vector math functions

	@author Conor MacKeigan 2017
*/

template <typename T>
struct Vector2
{
public:
	Vector2();
	Vector2(T x, T y);

	// For assigning different template types to each other
	template <typename U>
	Vector2(const Vector2<U>& other);
	
	T x;
	T y;

	// Unit vector declarations
	static const Vector2 zero;
	static const Vector2 left;
	static const Vector2 right;
	static const Vector2 up;
	static const Vector2 down;

	// Vector length
	T Magnitude() const;
	T SqrMagnitude() const;

	// Normalize the length of this vector
	void Normalize();

	// Return a normalized copy of this vector
	Vector2 Normalized() const;

	// Normalize and scale this vector to length
	void SetLength(T length);

	// Returns a copy of this vector of the specified length
	Vector2 AsLength(T length) const;

	// Dot product between two vectors
	static float Dot(const Vector2& lhs, const Vector2& rhs);

	// This vector's angle in radians
	float Angle() const;

	// Angle between two non-unit length vectors in radians
	static float Angle(const Vector2& lhs, const Vector2& rhs);

	// Angle between two unit-length vectors in radians
	static float AnglePrenorm(const Vector2& lhs, const Vector2& rhs);

	// Signed angle between two non-unit length vectors in radians
	static float SignedAngle(const Vector2& from, const Vector2& to);

	// Signed angle between two unit length vectors in radians
	static float SignedAnglePrenorm(const Vector2& from, const Vector2& to);

	// Rotate this vector around the origin
	void Rotate(float degrees);

	// Copy of a vector rotated around the origin
	static Vector2 Rotated(const Vector2& vector, float degrees);

	// Rotate this vector around a pivot point
	void RotateAround(const Vector2& pivot, float degrees);

	// Copy of a vector rotated around a pivot point
	static Vector2 RotatedAround(const Vector2& vector, const Vector2& pivot, float degrees);

	static Vector2 PerpendicularCounterClockwise(const Vector2& vector);

	static Vector2 PerpendicularClockwise(const Vector2& vector);
};


// Unit vectors
template <typename T> const Vector2<T> Vector2<T>::zero(0, 0);
template <typename T> const Vector2<T> Vector2<T>::right(1, 0);
template <typename T> const Vector2<T> Vector2<T>::left(-1, 0);
template <typename T> const Vector2<T> Vector2<T>::up(0, -1);
template <typename T> const Vector2<T> Vector2<T>::down(0, 1);


// Operators
template <typename T> Vector2<T>& operator+=(Vector2<T>& lhs, const Vector2<T>& rhs);
template <typename T> Vector2<T>& operator-=(Vector2<T>& lhs, const Vector2<T>& rhs);
template <typename T> Vector2<T>& operator*=(Vector2<T>& lhs, T rhs);
template <typename T> Vector2<T>& operator/=(Vector2<T>& lhs, T rhs);
template <typename T> Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs);
template <typename T> Vector2<T> operator-(const Vector2<T>& rhs);
template <typename T> Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs);
template <typename T> Vector2<T> operator*(T lhs, const Vector2<T>& rhs);
template <typename T> Vector2<T> operator*(const Vector2<T>& lhs, T rhs);
template <typename T> Vector2<T> operator/(const Vector2<T>& lhs, T rhs);
template <typename T> bool operator==(const Vector2<T>& lhs, const Vector2<T>& rhs);
template <typename T> bool operator!=(const Vector2<T>& lhs, const Vector2<T>& rhs);
template <typename T> bool operator<(const Vector2<T>& lhs, const Vector2<T>& rhs);
template <typename T> bool operator<=(const Vector2<T>& lhs, const Vector2<T>& rhs);
template <typename T> bool operator>(const Vector2<T>& lhs, const Vector2<T>& rhs);
template <typename T> bool operator>=(const Vector2<T>& lhs, const Vector2<T>& rhs);


// Common templates (these are the only types with vector math and operator fuctions defined)
typedef Vector2<float>				Vector2f;
typedef Vector2<int>				Vector2i;
typedef Vector2<unsigned int>		Vector2u;
