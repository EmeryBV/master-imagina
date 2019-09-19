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

vector<RGBf> read_palette_from_cin()
{
	string line;
	vector<RGBf> palette;
	
	RGBf color;

	while (cin >> color)
		palette.push_back(color);

	return std::move(palette);
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
	if (argc < 3)
    {
    	cerr << "Usage : " << argv[0] << " <image_in.ppm> <output[ppm|pgm]>\n";
		return EXIT_FAILURE;
	}

	string image_name = argv[1];
	string output_format = argv[2];

	if (output_format != "pgm" && output_format != "ppm")
	{
		cerr << "Erreur : les seules formats de sortie surporté sont pgm ou ppm\n";
		exit(EXIT_FAILURE);
	}

	cerr << "Lecture de l'image...\n";

	ImagePPM image_in;

	image_in.load(image_name);

	cerr << "Lecture de la palette...\n";
	
	vector<RGBf> palette = read_palette_from_cin();

	print(palette);
	
	cerr << "Classement des couleurs en fonction de la palette...\n";

	auto palette_color_map = map_color_with_palette_index(image_in, palette);

	cerr << "Trie des classes de couleurs...\n";

	for (auto& pc : palette_color_map)
		sort(pc.second.begin(), pc.second.end());

	cerr << "Remplacement des couleurs...\n";

	ImagePGM image_out_pgm(image_in.width(), image_in.height());
	ImagePPM image_out_ppm(image_in.width(), image_in.height());

	for (size_t px = 0; px < image_in.size(); ++px)
	{
		for (const auto& pc : palette_color_map)
		{
			if (binary_search(pc.second.begin(), pc.second.end(), image_in[px]))
			{
				image_out_pgm[px] = pc.first;
				image_out_ppm[px] = palette[pc.first];
			}
		}
	}

	cerr << "PSNR(in, out) = " << PSNR(image_in, image_out_ppm) << "dB\n";


	if (output_format == "pgm")
	{
		cerr << "Ecriture de l'image au format pgm...\n";
		cout << image_out_pgm << "\n";
	}
	else
	{
		cerr << "Ecriture de l'image au format ppm...\n";
		cout << image_out_ppm << "\n";
	}

	cerr << "Fin\n";

	return 0;
}
