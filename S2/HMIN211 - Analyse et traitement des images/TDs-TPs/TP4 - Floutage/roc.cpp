#include <iostream>
#include "Image.h"

using namespace std;

void count_truth_table(const ImagePGM& seuillee, const ImagePGM truth, long long truth_table[4]) {
	for (size_t i = 0 ; i < truth.size() ; ++i) {
		if (seuillee[i] > 128 && truth[i] > 128) { // vrai positif 255 && 255
			truth_table[0]++;
		} else if (seuillee[i] < 128 && truth[i] < 128) { // vrai negatif 0 && 0
			truth_table[1]++;
		} else if (seuillee[i] < 128 && truth[i] > 128) { // faux negatif 0 && 255
			truth_table[2]++;
		} else if (seuillee[i] > 128 && truth[i] < 128) { // faux positif 255 && 0
			truth_table[3]++;
		}
	}
}

int main(int argc, char** argv) {

	if (argc < 3) {
		std::cerr << "Usage: <image.pgm> <image_veritee.pgm>\n";
		return 1;
	}

	bool is_pgm = has_suffix(argv[1], ".pgm") && has_suffix(argv[2], ".pgm");

	if (!is_pgm) {
		std::cerr << "Erreur : le programme ne prend que des images au format PGM\n";
		return 1;
	}

	ImagePGM image(argv[1]);
	ImagePGM image_truth(argv[2]);

	if (!image.valid() || !image_truth.valid()) {
		std::cerr << "Erreur : image invalide\n";
		return 1;
	}

	ImagePGM image_seuillee = image;

	// 0 : vrai positif, 1 : vrai negatif, 2 : faux negatif, 3 : faux positif
	long long truth_table[4] = {0};

	for (int i = 0; i < 256; ++i){
		
		seuillage(image_seuillee, {i});
		count_truth_table(image_seuillee, image_truth, truth_table);
		
		image_seuillee = image;

		long long VP = truth_table[0];
		long long VN = truth_table[1];
		long long FN = truth_table[2];
		long long FP = truth_table[3];
		
		//cerr << "S = " << i << ", VP = " << VP << ", VN = " << VN << ", FN = " << FN << ", FP = " << FP << "\n";
		
		cout << VP / (double)(VP + FN) <<  " " << 1 - (VN / (double)(VN + FP)) << "\n";

		truth_table[0] = 0;
		truth_table[1] = 0;
		truth_table[2] = 0;
		truth_table[3] = 0;
	}

  return 0;
}