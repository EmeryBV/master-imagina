#include <iostream>
#include "Image.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc < 3) {
		std::cerr << "Usage: <image_ref.pgm/ppm> <image_spe.pgm/ppm>\n";
		return 1;
	}

	bool is_pgm = has_suffix(argv[1], ".pgm");
	bool is_ppm = has_suffix(argv[1], ".ppm");

	if (!is_pgm && !is_ppm) {
		std::cerr << "Erreur : le programme ne prend que des images au format PGM/PPM\n";
		return 1;
	}

	if (is_pgm) {

		ImagePGM image_ref(argv[1]);
		ImagePGM image_spe(argv[2]);

		if (!image_ref.valid() && !image_spe.valid()) {
			std::cerr << "Erreur : image invalide\n";
			return 1;
		}

		egalisation(image_spe, repartition_grey(ddp_grey(histo_grey(image_spe), image_spe.size())));

		std::vector<double> transform = repartition_grey(ddp_grey(histo_grey(image_ref), image_ref.size()));

		for (double& d : transform)
			d = d * 255.0;

		std::vector<double> transform_inv = repartition_inverse_grey(transform);

		for (OCTET& o : image_spe)
			o = transform_inv[o];

		cout << image_spe;

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