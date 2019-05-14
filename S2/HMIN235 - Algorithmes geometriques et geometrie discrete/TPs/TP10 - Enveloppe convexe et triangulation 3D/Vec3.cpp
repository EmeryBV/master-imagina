#include <cmath>
#include "Vec3.h"

Vec3 Vec3::zero    (0, 0, 0);
Vec3 Vec3::one     (1, 1, 1);

Vec3 Vec3::up      (0, 1, 0);
Vec3 Vec3::down    (0, -1, 0);
Vec3 Vec3::right   (1, 0, 0);
Vec3 Vec3::left    (-1, 0, 0);

Vec3 Vec3::forward (0, 0, 1);
Vec3 Vec3::backward(0, 0, -1);

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

Vec3& Vec3::normalize() {
	return *this /= length();
}

Vec3 Vec3::normalized() const {
	return *this / length();	
}

double Vec3::length() const {
	return pow(length_squared(), 0.5);
}

double Vec3::length_squared() const {
	return (x * x) + (y * y) + (z * z);
}

double Vec3::dot(Vec3& v) const {
	return (x * v.x) + (y * v.y) + (z * v.z);
}

Vec3 Vec3::cross(Vec3& v) const {
	return Vec3((y * v.z) - (z * v.y),
				(z * v.x) - (x * v.z),
				(x * v.y) - (y * v.x));
}

double Vec3::angle(Vec3& v) const {
	return acos(dot(v) / (length() * v.length()));
}

Vec3::operator double*() {
	return &x;
}

double Vec3::dot(const Vec3& left, const Vec3& right){
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z); 
}

Vec3 Vec3::min(const Vec3& left, const Vec3& right){
	return left.length_squared() < right.length_squared() ? left : right;
}

Vec3 Vec3::max(const Vec3& left, const Vec3& right){
	return left.length_squared() > right.length_squared() ? left : right;
}
////////////////////////////////////////////////// OPERATEURS DEFINITION

Vec3 operator-(const Vec3& left){
	return Vec3(-left.x, -left.y, -left.z);
}

Vec3& operator+=(Vec3& left, const Vec3& right){
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;
	return left;
}

Vec3& operator-=(Vec3& left, const Vec3& right){
	left.x -= right.x;
	left.y -= right.y;
	left.z -= right.z;
	return left;
}

Vec3 operator+(const Vec3& left, const Vec3& right){
	return Vec3(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vec3 operator-(const Vec3& left, const Vec3& right){
	return Vec3(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vec3 operator*(const Vec3& left, double right){
	return Vec3(left.x * right, left.y * right, left.z * right);
}

Vec3 operator*(double left, const Vec3& right){
	return (right * left);
}

Vec3& operator*=(Vec3& left, double right){
	left.x *= right;
	left.y *= right;
	left.z *= right;
	return left;
}

Vec3 operator/(const Vec3& left, double right){
	return Vec3(left.x / right, left.y / right, left.z / right);
}

Vec3& operator/=(Vec3& left, double right){
	left.x /= right;
	left.y /= right;
	left.z /= right;
	return left;
}

bool operator==(const Vec3& left, const Vec3& right){
	return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

bool operator!=(const Vec3& left, const Vec3& right){
	return !(left == right);
}

std::ostream& operator<<(std::ostream& os, const Vec3& v){
	return os << v.x << " " << v.y << " " << v.z;
}

std::istream& operator>>(std::istream& is, Vec3& v){
  return is >> v.x >> v.y >> v.z;
}
