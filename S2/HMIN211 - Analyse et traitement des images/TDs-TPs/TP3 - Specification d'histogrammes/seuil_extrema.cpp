#include <iostream>
#include "Image.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc < 4) {
		cerr << "Usage: <image.pgm/ppm> <rmin> <rmax> <gmin> <gmax> <bmin> <bmax>\n";
		return 1;
	}

	bool is_pgm = has_suffix(argv[1], ".pgm");
	bool is_ppm = has_suffix(argv[1], ".ppm");

	if (!is_pgm && !is_ppm) {
		cerr << "Erreur : le programme ne prend que des images au format PGM/PPM\n";
		return 1;
	}

	if (is_pgm) {

		ImagePGM image(argv[1]);

		if (!image.valid()) {
			cerr << "Erreur : image invalide\n";
			return 1;
		}

		vector<int> histo = histo_grey(image);

		int min = 0;
		int max = 255;

		min_max_grey(histo, min, max);

		cerr << "min = " <<  min << ", max = " << max << "\n";

		seuillage_extrema(image, atoi(argv[2]), atoi(argv[3]));

		cout << image;

	} else {

		if (argc < 8) {
			cerr << "Erreur : pour les images PPM il faut specifier les min max de chaque composantes\n";
			return 1;
		}

		ImagePPM image(argv[1]);

		if (!image.valid()) {
			cerr << "Erreur : image invalide\n";
			return 1;
		}

		ImagePGM red = image.red();
		ImagePGM green = image.green();
		ImagePGM blue = image.blue();

		vector<int> rhisto = histo_grey(red);
		vector<int> ghisto = histo_grey(green);
		vector<int> bhisto = histo_grey(blue);

		int rmin = 0;
		int rmax = 255;

		min_max_grey(rhisto, rmin, rmax);

		int gmin = 0;
		int gmax = 255;

		min_max_grey(ghisto, gmin, gmax);

		int bmin = 0;
		int bmax = 255;

		min_max_grey(bhisto, bmin, bmax);

		cerr << "rmin = " <<  rmin << ", rmax = " << rmax << "\n";
  		cerr << "gmin = " <<  gmin << ", gmax = " << gmax << "\n";
  		cerr << "bmin = " <<  bmin << ", bmax = " << bmax << "\n";

  		seuillage_extrema(red, atoi(argv[2]), atoi(argv[3]));
		seuillage_extrema(green, atoi(argv[4]), atoi(argv[5]));
		seuillage_extrema(blue, atoi(argv[6]), atoi(argv[7]));

		image.red(red);
		image.green(green);
		image.blue(blue);

		cout << image;
	}

  return 0;
}