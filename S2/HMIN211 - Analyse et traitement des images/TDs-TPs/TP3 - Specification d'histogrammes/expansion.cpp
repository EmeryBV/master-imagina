#include <iostream>
#include "Image.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc < 2) {
		cerr << "Usage: <image.pgm/ppm>\n";
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

		expansion(image, min, max);

		cout << image;

	} else {

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

		expansion(red, rmin, rmax);
		expansion(green, gmin, gmax);
		expansion(blue, bmin, bmax);

		image.red(red);
		image.green(green);
		image.blue(blue);

		cout << image;
	}

  return 0;
}