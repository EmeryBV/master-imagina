#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <GL/glut.h> 

struct Vec3
{
	double x, y, z;

	static Vec3 zero;
	static Vec3 one;

	static Vec3 up;
	static Vec3 down;
	static Vec3 left;
	static Vec3 right;
	
	static Vec3 forward;
	static Vec3 backward;

	Vec3();
	Vec3(double x, double y, double z = 0);

	void normalize();
	Vec3 normalized() const;

	double length() const;
	double length_squared() const;

	double dot(Vec3& v) const;
	Vec3 cross(Vec3& v) const;

	double angle(Vec3& v) const;

	operator double*();

	static double dot(const Vec3& left, const Vec3& right);
	static double cross(const Vec3& left, const Vec3& right);

	static double angle(const Vec3& from, const Vec3& to);
	static double distance(const Vec3& left, const Vec3& right);

	static Vec3 project(const Vec3& v, const Vec3& on_normal);
	static Vec3 project_on_plane(const Vec3& v, const Vec3& plane_normal);

	static Vec3 reflect(const Vec3& direction, const Vec3& normal);

	static Vec3 min(const Vec3& left, const Vec3& right);
	static Vec3 max(const Vec3& left, const Vec3& right);
};

Vec3 operator-(const Vec3& left);

Vec3& operator+=(Vec3& left, const Vec3& right);
Vec3& operator-=(Vec3& left, const Vec3& right);

Vec3 operator+(const Vec3& left, const Vec3& right);
Vec3 operator-(const Vec3& left, const Vec3& right);

Vec3 operator*(const Vec3& left, double right);
Vec3 operator*(double left, const Vec3& right);

Vec3& operator*=(Vec3& left, double right);

Vec3 operator/(const Vec3& left, double right);

Vec3& operator/=(Vec3& left, double right);

bool operator==(const Vec3& left, const Vec3& right);
bool operator!=(const Vec3& left, const Vec3& right);

std::ostream& operator<<(std::ostream& os, const Vec3& v);
std::istream& operator>>(std::istream& is, Vec3& v);

#endif