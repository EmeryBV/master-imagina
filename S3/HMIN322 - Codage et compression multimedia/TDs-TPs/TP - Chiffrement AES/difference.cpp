#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <array>
#include "../lib/Image.h"

using namespace std;

ImagePGM DIFF(const ImagePGM& left, const ImagePGM& right)
{
	assert(left.same_dimension(right));

	ImagePGM image_diff(left.width(), left.height());

	for (size_t i = 0; i < left.size(); ++i)
	{
		image_diff[i] = clamp(abs(left[i] - right[i]), 0, 255);
	}

	return std::move(image_diff);
}

int main(int argc, char** argv)
{
	if (argc < 3)
    {
    	cerr << "Usage : " << argv[0] << " <image_left.pgm> <image_right>\n";
    	cerr << "\n";
    	cerr << "Pour recuper une image en sortie il faudra faire une redirection de flux.\n";
    	cerr << "L'image d'entrée ne doit pas être la même que celle de sortie.\n";
    	cerr << "Exemple:\n";
    	cerr << "./différences image_left.pgm image_right.pgm > image_difference.pgm:\n";
		return EXIT_FAILURE;
	}

	ImagePGM image_left, image_right;

	cerr << "Lecture des images...\n";

	image_left.load(argv[1]);
	image_right.load(argv[2]);

	cerr << "Calcul de la différence entre les images...\n";

	ImagePGM image_out = DIFF(image_left, image_right);

	cerr << "Ecriture de l'image différence...\n";

	cout << image_out;

	return 0;
}
