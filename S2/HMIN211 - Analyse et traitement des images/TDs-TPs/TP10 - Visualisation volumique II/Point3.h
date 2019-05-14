#ifndef POINT3_H
#define POINT3_H

#include <iostream>

class Point3 {
public:
	double x, y, z;

	Point3();
	Point3(double x, double y, double z);
};

bool operator==(const Point3& left, const Point3& right);
bool operator!=(const Point3& left, const Point3& right);

std::ostream& operator<<(std::ostream& os, const Point3& p);
std::istream& operator>>(std::istream& is, Point3& p);

#endif