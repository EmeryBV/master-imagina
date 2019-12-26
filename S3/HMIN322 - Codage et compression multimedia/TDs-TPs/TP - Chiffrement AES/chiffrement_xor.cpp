#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <array>
#include "../lib/Image.h"

using namespace std;

ImagePGM random_image_seq(size_t width, size_t height, int seed)
{
	// force la graine à étre définie sur [0 100]
	seed = clamp(seed, 0, 100);

	ImagePGM random_seq(width, height);

	default_random_engine generator(seed);

	uniform_int_distribution<OCTET> pixel_distribution;
	
	for (OCTET& pixel : random_seq)
	{
		pixel = pixel_distribution(generator);
	}

	return std::move(random_seq);
}

ImagePGM XOR(const ImagePGM& left, const ImagePGM& right)
{
	assert(left.same_dimension(right));

	ImagePGM image_xor(left.width(), left.height());

	for (size_t i = 0; i < left.size(); ++i)
	{
		image_xor[i] = left[i] ^ right[i];
	}

	return std::move(image_xor);
}

double entropie_shannon(const ImagePGM& image)
{
	std::array<size_t, 256> pixel_occurence{0};

	double nb_pixels = image.size();

	for(const OCTET pixel : image)
	{
		pixel_occurence[pixel]++; 
	}

	double sum = 0;

	for(auto occurence : pixel_occurence)
	{
		double P_pixel = occurence / nb_pixels;

		if (occurence != 0)
			sum += P_pixel * log2(P_pixel);
	}

	return -sum;
}

int main(int argc, char** argv)
{
	if (argc < 3)
    {
    	cerr << "Usage : " << argv[0] << " <image_in.pgm> <random_seed>\n";
    	cerr << "\n";
    	cerr << "Pour recuper une image en sortie il faudra faire une redirection de flux.\n";
    	cerr << "L'image d'entrée ne doit pas être la même que celle de sortie.\n";
    	cerr << "Exemple:\n";
    	cerr << "./chiffrement_xor image.pgm 10 > image_chiffree.pgm:\n";
		return EXIT_FAILURE;
	}

	string image_name = argv[1];
	int seed = atoi(argv[2]);

	ImagePGM image_in;

	cerr << "Lecture de l'image...\n";

	image_in.load(image_name);

	cerr << "Generation de la sequence aléatoire...\n";

	ImagePGM random_seq = random_image_seq(image_in.width(), image_in.height(), seed);

	cerr << "Application de l'operation XOR...\n";

	ImagePGM image_out = XOR(image_in, random_seq);

	cerr << "Calcul de l'entrepie de l'image...\n";

	cerr << "entropie_shannon(image_out) = " << entropie_shannon(image_out) << " bpp\n";

	cerr << "Ecriture de l'image...\n";

	cout << image_out;

	return 0;
}
