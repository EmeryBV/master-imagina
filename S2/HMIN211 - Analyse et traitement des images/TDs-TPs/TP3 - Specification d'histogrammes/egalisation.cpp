#include <iostream>
#include "Image.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc < 2) {
		std::cerr << "Usage: <image.pgm/ppm>\n";
		return 1;
	}

	bool is_pgm = has_suffix(argv[1], ".pgm");
	bool is_ppm = has_suffix(argv[1], ".ppm");

	if (!is_pgm && !is_ppm) {
		std::cerr << "Erreur : le programme ne prend que des images au format PGM/PPM\n";
		return 1;
	}

	if (is_pgm) {

		ImagePGM image(argv[1]);

		if (!image.valid()) {
			std::cerr << "Erreur : image invalide\n";
			return 1;
		}

		egalisation(image, repartition_grey(ddp_grey(histo_grey(image), image.size())));

		cout << image;

	} else {

		ImagePPM image(argv[1]);

		if (!image.valid()) {
			std::cerr << "Erreur : image invalide\n";
			return 1;
		}

		std::cerr << "Erreur : non implementé\n";
		//print_histo_color(histo_color(image), image.size());

	}

  return 0;
}