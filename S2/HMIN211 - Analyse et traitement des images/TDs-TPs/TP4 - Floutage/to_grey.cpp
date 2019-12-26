#include <iostream>
#include "Image.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc < 2) {
		cerr << "Usage: <image.ppm>\n";
		return 1;
	}

	bool is_ppm = has_suffix(argv[1], ".ppm");

	if (!is_ppm) {
		cerr << "Erreur : le programme ne prend que des images au format PGM/PPM\n";
		return 1;
	}

	ImagePPM image_in(argv[1]);

	if (!image_in.valid()) {
		cerr << "Erreur : image invalide\n";
		return 1;
	}

	ImagePGM image_out(image_in.width(), image_in.height());

	for (size_t i = 0 ; i < image_in.height() ; ++i) {
		for (size_t j = 0 ; j < image_in.width() ; ++j) {
			image_out(i, j) = 0.299 * (double)image_in.red(i, j) + 0.587 * (double)image_in.green(i, j) + 0.114 * (double)image_in.blue(i, j);
		}
	}

	cout << image_out;

  return 0;
}