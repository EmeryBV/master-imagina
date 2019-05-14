#include "Point3.h"

Point3::Point3() : x(), y(), z() {}
Point3::Point3(double x, double y, double z) : x(x), y(y), z(z)  {}

bool operator==(const Point3& left, const Point3& right){
	return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

bool operator!=(const Point3& left, const Point3& right){
	return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
}

std::ostream& operator<<(std::ostream& os, const Point3& p){
	os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	return os;
}

std::istream& operator>>(std::istream& is, Point3& p){
  is >> p.x >> p.y >> p.z;
  return is;
}