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

// Entre [0, 8]
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

// renvoie l'image déchiffré avec  sa clef de chiffrement
ImagePGM dechiffrement_brute(const ImagePGM& image_chiffree, int& clef, double seuil)
{
	ImagePGM image_dechiffree(image_chiffree.width(), image_chiffree.height());

	double entropie_chiffree = entropie_shannon(image_chiffree);

	for (int i = 0 ; i <= 100; i++)
	{
		ImagePGM random_seq = random_image_seq(image_chiffree.width(), image_chiffree.height(), i);
		
		image_dechiffree = XOR(image_chiffree, random_seq);

		double entropie_dechiffre = entropie_shannon(image_dechiffree);
		
		if (entropie_dechiffre < entropie_chiffree - seuil)
		{
			clef = i;
			return std::move(image_dechiffree);
		}
	}

	clef = -1;
	return image_dechiffree;
}

int main(int argc, char** argv)
{
	if (argc < 2)
    {
    	cerr << "Usage : " << argv[0] << " <image_in.pgm> <seuil>\n";
    	cerr << "\n";
    	cerr << "Pour recuper une image en sortie il faudra faire une redirection de flux.\n";
    	cerr << "L'image d'entrée ne doit pas être la même que celle de sortie.\n";
    	cerr << "Exemple:\n";
    	cerr << "./dechiffrement_brute image_chiffree.pgm > image_originale.pgm:\n";
		return EXIT_FAILURE;
	}

	string image_name = argv[1];
	double seuil = 0.1;
	
	if (argc >= 3)
	{
		seuil = atof(argv[2]);
	}

	ImagePGM image_in;

	cerr << "Lecture de l'image...\n";

	image_in.load(image_name);

	cerr << "Déchiffrement brute de l'image...\n";

	int clef_chiffrement;

	ImagePGM image_out = dechiffrement_brute(image_in, clef_chiffrement, seuil);

	if (clef_chiffrement != -1)
	{
		cerr << "L'image à était chiffré avec la clef " << clef_chiffrement << "\n";
		cerr << "entropie_shannon(image_dechiffree) = " << entropie_shannon(image_out) << " bpp\n";
	}
	else
	{
		cerr << "ATTENTION : L'image n'as pas pu être déchiffrée !\n";
	}

	cerr << "Ecriture de l'image...\n";

	cout << image_out;

	return 0;
}
