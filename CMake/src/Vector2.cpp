#include "Vector2.h"
#include <math.h>
#include "StandardTypes.h"

//--------------------------------------------------
//	Constructors
//--------------------------------------------------
template<typename T>
Vector2<T>::Vector2() : x(0), y(0)
{
}
template Vector2f::Vector2();
template Vector2i::Vector2();
template Vector2u::Vector2();

template<typename T>
Vector2<T>::Vector2(T x, T y) : x(x), y(y) { }
template Vector2f::Vector2(float x, float y);
template Vector2i::Vector2(int x, int y);
template Vector2u::Vector2(unsigned int x, unsigned int y);

template<typename T>
template<typename U>
Vector2<T>::Vector2(const Vector2<U>& other)
{
	x = static_cast<T>(other.x);
	y = static_cast<T>(other.y);
}
template Vector2f::Vector2(const Vector2i& other);
template Vector2f::Vector2(const Vector2u& other);
template Vector2i::Vector2(const Vector2f& other);
template Vector2i::Vector2(const Vector2u& other);
template Vector2u::Vector2(const Vector2f& other);
template Vector2u::Vector2(const Vector2i& other);

//--------------------------------------------------
//	Member functions
//--------------------------------------------------
template <typename T>
T Vector2<T>::Magnitude() const
{ 
	return sqrt(x*x + y*y); 
}
template float Vector2f::Magnitude() const;
template int Vector2i::Magnitude() const;
template unsigned int Vector2u::Magnitude() const;

template <typename T>
T Vector2<T>::SqrMagnitude() const
{ 
	return (x*x + y*y); 
}
template float Vector2f::SqrMagnitude() const;
template int Vector2i::SqrMagnitude() const;
template unsigned int Vector2u::SqrMagnitude() const;

template <typename T>
void Vector2<T>::Normalize()
{
	T len = sqrt(x*x + y*y);

	// cannot normalize a zero vector
	if (len == 0)
		return;

	x /= len;
	y /= len;
}
template void Vector2<float>::Normalize();
template void Vector2i::Normalize();
template void Vector2u::Normalize();

// Return a normalized copy of this vector
template <typename T>
Vector2<T> Vector2<T>::Normalized() const
{
	T len = sqrt(x*x + y*y);
	return Vector2(x / len, y / len);
}
template Vector2f Vector2f::Normalized() const;
template Vector2i Vector2i::Normalized() const;
template Vector2u Vector2u::Normalized() const;

// Normalize and scale this vector to length
template <typename T>
void Vector2<T>::SetLength(T length)
{
	Normalize();
	x *= length;
	y *= length;
}
template void Vector2f::SetLength(float length);
template void Vector2i::SetLength(int length);
template void Vector2u::SetLength(unsigned int length);

// Returns a copy of this vector of the specified length
template <typename T>
Vector2<T> Vector2<T>::AsLength(T length) const
{
	Vector2 r = Vector2(*this);
	return r.Normalized() * length;
}
template Vector2f Vector2f::AsLength(float length) const;
template Vector2i Vector2i::AsLength(int length) const;
template Vector2u Vector2u::AsLength(unsigned int length) const;

// Dot product between two vectors
template <typename T>
float Vector2<T>::Dot(const Vector2& lhs, const Vector2& rhs)
{ 
	return lhs.x*rhs.x + lhs.y*rhs.y; 
}
template float Vector2f::Dot(const Vector2f& lhs, const Vector2f& rhs);
template float Vector2i::Dot(const Vector2i& lhs, const Vector2i& rhs);
template float Vector2u::Dot(const Vector2u& lhs, const Vector2u& rhs);

// This vector's angle in radians
template <typename T>
float Vector2<T>::Angle() const
{ 
	return atan2(y, x); 
}
template float Vector2f::Angle() const;
template float Vector2i::Angle() const;
template float Vector2u::Angle() const;

// Angle between two non-unit length vectors in radians
template <typename T>
float Vector2<T>::Angle(const Vector2& lhs, const Vector2& rhs)
{ 
	return acos(Dot(lhs.Normalized(), rhs.Normalized())); 
}
template float Vector2f::Angle(const Vector2f& lhs, const Vector2f& rhs);
template float Vector2i::Angle (const Vector2i& lhs, const Vector2i& rhs);
template float Vector2u::Angle(const Vector2u& lhs, const Vector2u& rhs);

// Angle between two unit-length vectors in radians
template <typename T>
float Vector2<T>::AnglePrenorm(const Vector2& lhs, const Vector2& rhs)
{
	return acos(Dot(lhs, rhs)); 
}
template float Vector2f::AnglePrenorm(const Vector2f& lhs, const Vector2f& rhs);
template float Vector2i::AnglePrenorm(const Vector2i& lhs, const Vector2i& rhs);
template float Vector2u::AnglePrenorm(const Vector2u& lhs, const Vector2u& rhs);

// Signed angle between two non-unit length vectors in radians
template <typename T>
float Vector2<T>::SignedAngle(const Vector2& from, const Vector2& to)
{
	int sign = sign(Dot(PerpendicularCounterClockwise(from), to));
	float angle = Angle(from, to);

	return sign * angle;
}
template float Vector2f::SignedAngle(const Vector2f& lhs, const Vector2f& rhs);
template float Vector2i::SignedAngle(const Vector2i& lhs, const Vector2i& rhs);
template float Vector2u::SignedAngle(const Vector2u& lhs, const Vector2u& rhs);

// Signed angle between two unit length vectors in radians
template <typename T>
float Vector2<T>::SignedAnglePrenorm(const Vector2& from, const Vector2& to)
{
	int sign = sign(Dot(PerpendicularCounterClockwise(from), to));
	float angle = AnglePrenorm(from, to);

	return sign * angle;
}
template float Vector2f::SignedAnglePrenorm(const Vector2f& lhs, const Vector2f& rhs);
template float Vector2i::SignedAnglePrenorm(const Vector2i& lhs, const Vector2i& rhs);
template float Vector2u::SignedAnglePrenorm(const Vector2u& lhs, const Vector2u& rhs);

// Rotate this vector around the origin
template <typename T>
void Vector2<T>::Rotate(float degrees)
{
	float rads = DEG_TO_RAD(degrees);
	float s = sin(rads);
	float c = cos(rads);

	float px = x*c - y*s;
	float py = x*s + y*c;

	x = px;
	y = py;
}
template void Vector2f::Rotate(float degrees);
template void Vector2i::Rotate(float degrees);
template void Vector2u::Rotate(float degrees);

// Copy of a vector rotated around the origin
template <typename T>
Vector2<T> Vector2<T>::Rotated(const Vector2& vector, float degrees)
{
	Vector2 r = Vector2(vector);
	r.Rotate(degrees);

	return r;
}
template Vector2f Vector2f::Rotated(const Vector2f& vector, float degrees);
template Vector2i Vector2i::Rotated(const Vector2i& vector, float degrees);
template Vector2u Vector2u::Rotated(const Vector2u& vector, float degrees);

// Rotate this vector around a pivot point
template <typename T>
void Vector2<T>::RotateAround(const Vector2 & pivot, float degrees)
{
	// Rotate displacement vector then add it back to the pivot
	x -= pivot.x;
	y -= pivot.y;

	Rotate(degrees);

	x += pivot.x;
	y += pivot.y;
}
template void Vector2f::RotateAround(const Vector2& pivot, float degrees);
template void Vector2i::RotateAround(const Vector2& pivot, float degrees);
template void Vector2u::RotateAround(const Vector2& pivot, float degrees);

// Copy of a vector rotated around a pivot point
template <typename T>
Vector2<T> Vector2<T>::RotatedAround(const Vector2& vector, const Vector2& pivot, float degrees)
{
	return Rotated(vector - pivot, degrees) + pivot;
}
template Vector2f Vector2f::RotatedAround(const Vector2& vector, const Vector2& pivot, float degrees);
template Vector2i Vector2i::RotatedAround(const Vector2& vector, const Vector2& pivot, float degrees);
template Vector2u Vector2u::RotatedAround(const Vector2& vector, const Vector2& pivot, float degrees);

template <typename T>
Vector2<T> Vector2<T>::PerpendicularCounterClockwise(const Vector2& vector)
{
	return Vector2(vector.y, -vector.x);
}
template Vector2f Vector2f::PerpendicularCounterClockwise(const Vector2& vector);
template Vector2i Vector2i::PerpendicularCounterClockwise(const Vector2& vector);

template <typename T>
Vector2<T> Vector2<T>::PerpendicularClockwise(const Vector2& vector)
{
	return Vector2(-vector.y, vector.x);
}
template Vector2f Vector2f::PerpendicularClockwise(const Vector2& vector);
template Vector2i Vector2i::PerpendicularClockwise(const Vector2& vector);


//-------------------------------------------------------------------
//	Operators (ignore the green underlines, intellisense is trippin)
//-------------------------------------------------------------------
template <typename T> Vector2<T>& operator+=(Vector2<T>& lhs, const Vector2<T>& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
template Vector2f& operator+=(Vector2f& lhs, const Vector2f& rhs);
template Vector2i& operator+=(Vector2i& lhs, const Vector2i& rhs);
template Vector2u& operator+=(Vector2u& lhs, const Vector2u& rhs);

template <typename T> Vector2<T>& operator-=(Vector2<T>& lhs, const Vector2<T>& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
template Vector2f& operator-=(Vector2f& lhs, const Vector2f& rhs);
template Vector2i& operator-=(Vector2i& lhs, const Vector2i& rhs);
template Vector2u& operator-=(Vector2u& lhs, const Vector2u& rhs);

template <typename T> Vector2<T>& operator*=(Vector2<T>& lhs, T rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
template Vector2f& operator*=(Vector2f& lhs, float rhs);
template Vector2i& operator*=(Vector2i& lhs, int rhs);
template Vector2u& operator*=(Vector2u& lhs, unsigned int rhs);

template <typename T> Vector2<T>& operator/=(Vector2<T>& lhs, T rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
template Vector2f& operator/=(Vector2f& lhs, float rhs);
template Vector2i& operator/=(Vector2i& lhs, int rhs);
template Vector2u& operator/=(Vector2u& lhs, unsigned int rhs);

template <typename T> Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y); }
template Vector2f operator+(const Vector2f& lhs, const Vector2f& rhs);
template Vector2i operator+(const Vector2i& lhs, const Vector2i& rhs);
template Vector2u operator+(const Vector2u& lhs, const Vector2u& rhs);

template <typename T> Vector2<T> operator-(const Vector2<T>& rhs) { return Vector2<T>(-rhs.x, -rhs.y); }
template Vector2f operator-(const Vector2f& rhs);
template Vector2i operator-(const Vector2i& rhs);
template Vector2u operator-(const Vector2u& rhs);

template <typename T> Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y); }
template Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs);
template Vector2i operator-(const Vector2i& lhs, const Vector2i& rhs);
template Vector2u operator-(const Vector2u& lhs, const Vector2u& rhs);

template <typename T> Vector2<T> operator*(T lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs * rhs.x, lhs * rhs.y); }
template Vector2f operator*(float lhs, const Vector2f& rhs);
template Vector2i operator*(int lhs, const Vector2i& rhs);
template Vector2u operator*(unsigned int lhs, const Vector2u& rhs);

template <typename T> Vector2<T> operator*(const Vector2<T>& lhs, T rhs) { return Vector2<T>(lhs.x * rhs, lhs.y * rhs); }
template Vector2f operator*(const Vector2f& lhs, float rhs);
template Vector2i operator*(const Vector2i& lhs, int rhs);
template Vector2u operator*(const Vector2u& lhs, unsigned int rhs);

template <typename T> Vector2<T> operator/(const Vector2<T>& lhs, T rhs) { return Vector2<T>(lhs.x / rhs, lhs.y / rhs); }
template Vector2f operator/(const Vector2f& lhs, float rhs);
template Vector2i operator/(const Vector2i& lhs, int rhs);
template Vector2u operator/(const Vector2u& lhs, unsigned int rhs);

template <typename T> bool operator==(const Vector2<T>& lhs, const Vector2<T>& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
template bool operator==(const Vector2f& lhs, const Vector2f& rhs);
template bool operator==(const Vector2i& lhs, const Vector2i& rhs);
template bool operator==(const Vector2u& lhs, const Vector2u& rhs);

template <typename T> bool operator!=(const Vector2<T>& lhs, const Vector2<T>& rhs) { return lhs.x != rhs.x || lhs.y != rhs.y; }
template bool operator!=(const Vector2f& lhs, const Vector2f& rhs);
template bool operator!=(const Vector2i& lhs, const Vector2i& rhs);
template bool operator!=(const Vector2u& lhs, const Vector2u& rhs);

template <typename T> bool operator<(const Vector2<T>& lhs, const Vector2<T>& rhs) { return lhs.SqrMagnitude() < rhs.SqrMagnitude(); }
template bool operator<(const Vector2f& lhs, const Vector2f& rhs);
template bool operator<(const Vector2i& lhs, const Vector2i& rhs);
template bool operator<(const Vector2u& lhs, const Vector2u& rhs);

template <typename T> bool operator<=(const Vector2<T>& lhs, const Vector2<T>& rhs) { return lhs.SqrMagnitude() <= rhs.SqrMagnitude(); }
template bool operator<=(const Vector2f& lhs, const Vector2f& rhs);
template bool operator<=(const Vector2i& lhs, const Vector2i& rhs);
template bool operator<=(const Vector2u& lhs, const Vector2u& rhs);

template <typename T> bool operator>(const Vector2<T>& lhs, const Vector2<T>& rhs) { return lhs.SqrMagnitude() > rhs.SqrMagnitude(); }
template bool operator>(const Vector2f& lhs, const Vector2f& rhs);
template bool operator>(const Vector2i& lhs, const Vector2i& rhs);
template bool operator>(const Vector2u& lhs, const Vector2u& rhs);

template <typename T> bool operator>=(const Vector2<T>& lhs, const Vector2<T>& rhs) { return lhs.SqrMagnitude() >= rhs.SqrMagnitude(); }
template bool operator>=(const Vector2f& lhs, const Vector2f& rhs);
template bool operator>=(const Vector2i& lhs, const Vector2i& rhs);
template bool operator>=(const Vector2u& lhs, const Vector2u& rhs);