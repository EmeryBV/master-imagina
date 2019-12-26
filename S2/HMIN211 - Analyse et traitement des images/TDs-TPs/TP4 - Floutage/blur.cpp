#include <iostream>
#include "Image.h"

using namespace std;

OCTET moyenne(const ImagePGM& im, size_t row, size_t column) {
	
	long sum = im(row, column);
	size_t nb_values = 1;

	// dans le sens horraire
	
	if (im.on_bounds(row-1, column)) {
		sum += im(row-1, column);
		nb_values++;
	}

	if (im.on_bounds(row-1, column+1)) {
		sum += im(row-1, column+1);
		nb_values++;
	}

	if (im.on_bounds(row, column+1)) {
		sum += im(row, column+1);
		nb_values++;
	}

	if (im.on_bounds(row-1, column+1)) {
		sum += im(row-1, column+1);
		nb_values++;
	}

	if (im.on_bounds(row-1, column)) {
		sum += im(row-1, column);
		nb_values++;
	}

	if (im.on_bounds(row-1, column-1)) {
		sum += im(row-1, column-1);
		nb_values++;
	}

	if (im.on_bounds(row, column-1)) {
		sum += im(row, column-1);
		nb_values++;
	}

	if (im.on_bounds(row+1, column-1)) {
		sum += im(row+1, column-1);
		nb_values++;
	}

	return sum / nb_values;
}

void blur(const ImagePPM& in, ImagePPM& out) {
	for (size_t i = 0 ; i < in.height() ; ++i) {
		for (size_t j = 0 ; j < in.width() ; ++j) {
			out.red(i, j) = moyenne(in.red(), i, j);
			out.green(i, j) = moyenne(in.green(), i, j);
			out.blue(i, j) = moyenne(in.blue(), i, j);
		}
	}
}

//blur_background

void blur(const ImagePPM& in, const ImagePGM& bin, ImagePPM& out) {
	for (size_t i = 0 ; i < in.height() ; ++i) {
		for (size_t j = 0 ; j < in.width() ; ++j) {
			if (bin(i,j) != 0) {
				out.red(i, j) = moyenne(in.red(), i, j);
				out.green(i, j) = moyenne(in.green(), i, j);
				out.blue(i, j) = moyenne(in.blue(), i, j);
			} else {
				out.red(i, j) = in.red(i, j);
				out.green(i, j) = in.green(i, j);
				out.blue(i, j) = in.blue(i, j);			
			}
		}
	}
}

int main(int argc, char** argv) {

	if (argc < 2) {
		cerr << "Usage: <image.ppm> <NB_BLUR> <OPT:image_bin.pgm>\n";
		return 1;
	}

	bool is_ppm = has_suffix(argv[1], ".ppm");

	if (!is_ppm ) {
		cerr << "Erreur : le programme ne prend que des images au format PPM\n";
		return 1;
	}

	ImagePPM image_in(argv[1]);

	if (!image_in.valid()) {
		cerr << "Erreur : image PPM invalide\n";
		return 1;
	}

	int nb_blur = 1;

	if (argc >= 3)
		nb_blur = atoi(argv[2]);

	ImagePPM image_out(image_in.width(), image_in.height());

	if (argc >= 4) {

		ImagePGM image_bin(argv[3]);

		if (!image_bin.valid()) {
			cerr << "Erreur : image PGM invalide\n";
			return 1;
		}

		for (int i = 0; i < nb_blur; ++i){
			blur(image_in, image_bin, image_out);
			image_in = image_out;
		}

	} else {

		for (int i = 0; i < nb_blur; ++i){
			blur(image_in, image_out);
			image_in = image_out;
		}
	}

	cout << image_out;

  return 0;
}