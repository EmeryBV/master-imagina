#include <iostream> 
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include "Image.h"
#include "Vec3.h"

using namespace std;

// Structures

typedef Vec3 Point3;

struct Triangle
{
	Point3 a, b, c;

	Triangle() : a(), b(), c() {}
	Triangle(Point3 a, Point3 b, Point3 c) : a(a), b(b), c(c) {}
};

// initialise les points de manière aléatoires dans le cercle de centre 'center' et de rayon 'radius'
void randomize_points(vector<Point3>& points, Point3 center, int radius)
{
	default_random_engine generator(time(0));

	uniform_real_distribution<double> angle_distribution(0, 2 * M_PI);
	uniform_real_distribution<double> radius_distribution(0, 1);
	
	for (Point3& p : points)
	{
		double random_angle = angle_distribution(generator);

		// sqrt() permet de rendre la distribution uniforme dans le rayon. 
		double random_radius = sqrt(radius_distribution(generator)) * radius;

		p.x = center.x + cos(random_angle) * random_radius;
		p.y = center.y + sin(random_angle) * random_radius; 
	} 
}

double determinant_2d(Vec3 U, Vec3 V)
{
	return U.cross(V).z;
}

double determinant(Vec3 U, Vec3 V)
{
	return U.cross(V).length();
}

bool concave(Point3 r, Point3 s, Point3 t)
{
	Vec3 rs = s - r;
	Vec3 rt = t - r;

	return determinant_2d(rs, rt) <= 0;
}

// calcul une enveloppe convexe des points, ne marche que en 2 dimensions.
vector<int> graham(vector<Point3>& points)
{
	// recherche du pivot
	auto pivot_iter = min_element(points.begin(), points.end(), [](Point3& p1, Point3& p2) -> bool
	{
		return (p1.x < p2.x) || (p1.x <= p2.x && p1.y > p2.y); // Point le plus en droite en bas de l'image.
	});

	Point3 pivot = *pivot_iter;
	iter_swap(points.begin(), pivot_iter);

	// trie des point selon l'angle du vecteur pivot/point
	sort(points.begin() + 1, points.end(), [pivot](Point3& p1, Point3& p2) -> bool
	{
		Vec3 pivot_p1 = (p1 - pivot);
		Vec3 pivot_p2 = (p2 - pivot);

		double angle_pivot_p1 = atan2(pivot_p1.y, pivot_p1.x); 
		double angle_pivot_p2 = atan2(pivot_p2.y, pivot_p2.x); 

		if (angle_pivot_p1 == angle_pivot_p2)
		{
			return pivot_p1.length_squared() < pivot_p2.length_squared(); // on prend le point le plus loin si les angles sont egaux
		}

		return angle_pivot_p1 < angle_pivot_p2;
	});

	// le pivot est forcément dans le parcours
	vector<int> parcours = { 0, 1 }; // le parcours contient les indices des points

	for (size_t i = 2 ; i < points.size() ; ++i)
	{
		// on prend un nouveau point quand la figure formé n'est pas concave
		while (parcours.size() > 1 && concave(points[i], points[parcours[parcours.size() - 2]], points[parcours.back()]))
		{
			parcours.pop_back();
		}

		parcours.push_back(i);
	}

	parcours.push_back(0); // boucle le parcours

	return std::move(parcours);
}

bool visible(Point3 O, Point3 Pi, Point3 Pj, Point3 Pk)
{
	Vec3 O_Pj = Pj - O;
	Vec3 O_Pk = Pk - O;

	Vec3 Pi_Pj = Pj - Pi;
	Vec3 Pi_Pk = Pk - Pi;

	return determinant_2d(O_Pj, O_Pk) * determinant_2d(Pi_Pj, Pi_Pk) < 0;
}

// calcul une enveloppe convexe des points, ne marche que en 2 dimensions.
vector<int> incremental_2d(vector<Point3>& points)
{
	// trie des point selon l'angle du vecteur pivot/point
	sort(points.begin(), points.end(), [](Point3& p1, Point3& p2) -> bool
	{
		return  (p1.x < p2.x) || (p1.x <= p2.x && p1.y < p2.y); // Point le plus en a droite en bas de l'image.
	});

	vector<int> enveloppe = { 0, 1, 2 }; // l'enveloppe contient 3 indices pour commencer

	Point3 Oppose = points[enveloppe[0]];

	for (size_t i = 3 ; i < points.size() ; ++i)
	{
		// on prend un nouveau point quand la figure formé n'est pas concave
		while (enveloppe.size() > 1 && visible(points[enveloppe[0]], points[i], points[]))
		{
			enveloppe.pop_back();
		}

		enveloppe.push_back(i);
	}

	enveloppe.push_back(0); // boucle le parcours

	return std::move(enveloppe);
}

// ecriture des points et du parcours aux format ply
ostream& write_ply(ostream& out, const vector<Point3>& points, const vector<int>& parcours)
{
	out << "ply\n";
	out << "format ascii 1.0\n";

	out << "comment this file contains points and lines.\n";

	out << "element vertex " << points.size() << "\n";
	out << "property float x\n";
	out << "property float y\n";
	out << "property float z\n";

	out << "element edge " << parcours.size() - 1 << "\n";
	out << "property int vertex1\n";
	out << "property int vertex2\n";

	out << "end_header\n";

	// ecriture des points
	for (Point3 p : points)
		out << p.x << " " << p.y << " " << p.z << "\n";

	// ecriture des arêtes
	for (size_t i = 0 ; i < parcours.size() - 1 ; ++i)
		out << parcours[i] << " " << parcours[i + 1] << "\n";

	return out;
}

int main(int argc, char const *argv[])
{
	if (argc < 4)
	{
		cerr << "Usage : " << argv[0] << " <x_max> <y_max> <nb_points>\n";
		exit(EXIT_FAILURE);
	}

	size_t x_max = atoi(argv[1]);
	size_t y_max = atoi(argv[2]);
	size_t nb_points = atoi(argv[3]);

	ImagePGM out(x_max + 1, y_max + 1);

	vector<Point3> points(nb_points);

	size_t radius = min(x_max, y_max) / 2;
	Point3 center(x_max / 2, y_max / 2);

	randomize_points(points, center, radius);

	for (Point3 p : points)
		out(p.y, p.x) = 255;

	vector<int> parcours;

	parcours = graham(points);

	write_ply(cout, points, parcours); // ecriture du parcours et des points en ply

	return 0;
}

