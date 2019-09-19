#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <experimental/filesystem>
#include "../lib/Image.h"
// #include "Image.h"

using namespace std;

double color_distance(const RGB& c1, const RGB& c2)
{
	return sqrt(pow(c1.r - c2.r, 2) + pow(c1.g - c2.g, 2) + pow(c1.b - c2.b, 2));
}

template<typename T>
T average(const vector<T>& v)
{
	T sum;
	for (const T& val : v)
		sum += val;

	return sum / v.size();
}

template<typename T>
void print(const vector<T>& v)
{
	for (const T& val : v)
	{
		cerr << val << "\n";
	}
}

double EQM(const ImagePPM& left, const ImagePPM& right)
{
	if (!left.same_dimension(right))
	{
		cerr << "Error : Image of different dimension\n";
		exit(EXIT_FAILURE);
	}

	double eqm_r = 0;
	double eqm_g = 0;
	double eqm_b = 0;

	for (size_t px = 0 ; px < left.size() ; px++)
	{
		eqm_r += pow(left[px].r - right[px].r, 2);
		eqm_g += pow(left[px].g - right[px].g, 2);
		eqm_b += pow(left[px].b - right[px].b, 2);
	}

	return (eqm_r + eqm_g + eqm_b) / (left.size() * 3);
}

double PSNR(const ImagePPM& left, const ImagePPM& right)
{
	return 10 * log10(pow(255, 2) / EQM(left, right));
}


std::map<size_t, vector<RGBf>> map_color_with_palette_index(const ImagePPM& img, const vector<RGBf>& palette)
{
	map<size_t, vector<RGBf>> color_palette_map;

	for (size_t px = 0 ; px < img.size() ; px++)
	{
		double min_dist = std::numeric_limits<double>::max();
		size_t min_dist_index = 0;

		for (size_t i = 0 ; i < palette.size() ; i++)
		{
			double current_dist = color_distance(palette[i], img[px]);
			
			if (current_dist < min_dist)
			{
				min_dist = current_dist;
				min_dist_index = i;
			}
		}

		color_palette_map[min_dist_index].push_back(img[px]);
	}

	return std::move(color_palette_map);
}


int main(int argc, char** argv)
{
	if (argc < 2)
    {
    	cerr << "Usage : " << argv[0] << " <image_in.ppm> <nb_couleurs_palette> <nb_iterations_k-mean>\n";
		return EXIT_FAILURE;
	}

	string image_name = argv[1];
	size_t nb_couleurs_palette = 256;
	int nb_iterations = 10;
	
	if (argc > 2)
		nb_couleurs_palette = atoi(argv[2]);

	if (argc > 3)
		nb_iterations = atoi(argv[3]);

	ImagePPM image_in;

	cerr << "Lecture de l'image...\n";

	image_in.load(image_name);

	// Creation de l'image de sortie

	ImagePPM image_out = image_in;

	cerr << "Selection d'une palette de couleurs au hasard...\n";

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::shuffle(image_out.begin(), image_out.end(), std::default_random_engine(seed));
	
	vector<RGBf> palette(nb_couleurs_palette);

	std::copy(image_out.begin(), image_out.begin() + palette.size(), palette.begin());

	map<size_t, vector<RGBf>> palette_color;

	cerr << "Application de l'algorithme k-mean...\n";
	
	for (int k = 0 ; k < nb_iterations ; k++)
	{		
		// Création des classes de couleurs
	
		palette_color.clear();

		palette_color = map_color_with_palette_index(image_in, palette);

		// Calcul de la couleur moyenne dans chaque classes de couleurs

		for (const auto& pc : palette_color)
			palette[pc.first] = average(pc.second);

		// cerr << "Palette N°" << k << "\n";
		// print(palette);
	}

	cerr << "Ecriture de la palette...\n";

	for (size_t i = 0 ; i < palette.size() ; i++)
	{
		cout << palette[i] << "\n";
	}

	cerr << "Fin\n";

	return 0;
}
