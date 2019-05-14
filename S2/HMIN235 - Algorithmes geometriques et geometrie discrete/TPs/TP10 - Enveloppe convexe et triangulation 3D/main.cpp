#include <iostream> 
#include <fstream>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include <sstream>
#include <array>
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
void randomize_points(vector<Point3>& points, Point3 center, size_t radius)
{
	default_random_engine generator(time(0));
	uniform_real_distribution<double> random(0, 1);
	
	for (Point3& p : points)
	{
		// (u, v, w) ∈ [-1, 1]
		double u = 2 * random(generator) - 1; 
		double v = 2 * random(generator) - 1;
		double w = 0;
		// double w = 2 * random(generator) - 1;
		
		// r ∈ [0, radius]
		double r = pow(random(generator), 1. / 2.) * radius;
		// double r = pow(random(generator), 1. / 3.) * radius;

		p = center + r * Vec3(u, v, w).normalize();
	}
}

double determinant_2d(Vec3 U, Vec3 V)
{
	return U.cross(V).z;
}

double determinant(Vec3 U, Vec3 V)
{
	return U.cross(V).length_squared();
}

bool is_visible(Point3 O, Point3 Pj, Point3 Pk, Point3 Pi)
{
	Vec3 O_Pj = Pj - O;
	Vec3 O_Pk = Pk - O;

	Vec3 Pi_Pj = Pj - Pi;
	Vec3 Pi_Pk = Pk - Pi;

	return determinant_2d(O_Pj, O_Pk) * determinant_2d(Pi_Pj, Pi_Pk) < 0;
}

// calcul une enveloppe convexe des points, ne marche que en 2 dimensions.
vector<size_t> incremental_2d(vector<Point3>& points)
{
	// trie des point selon l'angle du vecteur pivot/point
	sort(points.begin(), points.end(), [](Point3& p1, Point3& p2) -> bool
	{
		return  (p1.x < p2.x) || (p1.x <= p2.x && p1.y < p2.y); // Point le plus en a droite en bas de l'image.
	});

	// vector<size_t> enveloppe = {0, 1, 2}; // l'enveloppe contient un triangle pour commencer
	vector<array<size_t, 2> > aretes = {{0, 1}, {1, 2}, {2, 0}};

	Point3 triangle_center = (points[0] + points[1] + points[2]) / 3;

/*
	for (size_t i = 3 ; i < points.size() ; ++i)
	{
		while (aretes.size() > 2 && )
		{
			aretes.pop_back();
		}
		aretes.push_back(i);
	}
*/
	vector<array<size_t, 2> > add;

	for (size_t j = 0 ; j < aretes.size() ; ++j)
	{
		if(!is_visible(triangle_center, points[aretes[j][0]], points[aretes[j][1]], points[3]))
		{
		}
			add.push_back({aretes[j][0], 3});
			add.push_back({3, aretes[j][1]});
	}

	for (auto edge : add)
	{
		aretes.push_back(edge);
	}

	vector<size_t> enveloppe;

	for(auto arete : aretes)
	{
		enveloppe.push_back(arete[0]);
		cerr << arete[0] << " ";
	}
	cerr << "\n";

	return std::move(enveloppe);
}

// ecriture des points et du parcours aux format ply dans un flux
ostream& write_ply(ostream& out, const vector<Point3>& points, const vector<size_t>& enveloppe)
{
	out << "ply\n";
	out << "format ascii 1.0\n";

	out << "comment this file contains points and lines.\n";

	out << "element vertex " << points.size() << "\n";
	out << "property float x\n";
	out << "property float y\n";
	out << "property float z\n";

	out << "element edge " << enveloppe.size() << "\n";
	out << "property int vertex1\n";
	out << "property int vertex2\n";

	out << "end_header\n";

	// ecriture des points
	for (Point3 p : points)
		out << p.x << " " << p.y << " " << p.z << "\n";

	// ecriture des arêtes
	for (size_t i = 1 ; i < enveloppe.size() ; ++i)
		out << enveloppe[i - 1] << " " << enveloppe[i] << "\n";

	// ecriture de l'arete bouclant l'enveloppe convexe
	if (enveloppe.size() > 2)
		out << enveloppe[enveloppe.size() - 1] << " " << enveloppe[0] << "\n";

	return out;
}

int main(int argc, char const *argv[])
{
	if (argc < 4)
	{
		cerr << "Usage : " << argv[0] << " <x_max> <y_max> <nb_points>\n";
		exit(EXIT_FAILURE);
	}

	// paramètres du programme

	size_t x_max = atoi(argv[1]);
	size_t y_max = atoi(argv[2]);
	size_t nb_points = atoi(argv[3]);

	// création du fichier de sortie

	stringstream file_name;

	file_name << x_max << "_" << y_max << "_" << nb_points << ".ply";

	ofstream file_out(file_name.str());

	if (!file_out.is_open())
	{
		cerr << "Erreur à l'ouverture du fichier\n";
		exit(EXIT_FAILURE);
	}

	// randomisation des points

	vector<Point3> points(nb_points);

	size_t radius = min(x_max, y_max) / 2;
	Point3 center(x_max / 2, y_max / 2);

	randomize_points(points, center, radius);

	// calcul de l'enveloppe convexe

	vector<size_t> enveloppe;
	
	enveloppe = incremental_2d(points);

	write_ply(file_out, points, enveloppe);

	return 0;
}

