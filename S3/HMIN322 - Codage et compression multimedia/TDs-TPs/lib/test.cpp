#include <iostream>
#include "Image.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 2)
    {
		printf("Usage: <ImageIn.ppm>\n");
		return EXIT_FAILURE;
	}

	cerr << "Lecture de l'image...\n";

	ImagePPM image_in; 
	RGB c1 = RGB::white;
	RGBf c2 = RGBf::black;

	cerr << (c1 < c2) << "\n";

	image_in.load(argv[1]);

	cerr << "Ecriture de l'image...\n";

	// cout << image_in << "\n";

	cerr << "Fin\n";


	return 0;
}
