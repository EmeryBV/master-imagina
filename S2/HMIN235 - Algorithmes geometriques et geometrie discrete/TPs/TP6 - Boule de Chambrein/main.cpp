#include <iostream> 
#include <cmath>
#include "Image.h"

using namespace std;


void set_neighbors(ImagePGM& image, int i, int j, int value, int offset) {
	image(i + offset, j) = value;
	image(i - offset, j) = value;
	image(i, j + offset) = value;
	image(i, j - offset) = value;
}

void set_neighbors_diag(ImagePGM& image, int i, int j, int value, int offset) {
	image(i + offset, j + offset) = value;
	image(i - offset, j + offset) = value;
	image(i - offset, j - offset) = value;
	image(i + offset, j - offset) = value;
}

size_t nb_neighbors(ImagePGM& image, int i, int j) {
	
	size_t neighbors = 0;
	
	if (image(i + 1, j) == 0) {
		neighbors++;
	}

	if (image(i - 1, j) == 0) {
		neighbors++;
	}

	if (image(i, j + 1) == 0) {
		neighbors++;
	}

	if (image(i, j - 1) == 0) {
		neighbors++;
	}

	return neighbors;
}

void init_chamfrein(ImagePGM& out, vector<int>& params, size_t radius) {
	out.resize(radius * 2 + 1, radius * 2 + 1);

	for (size_t i = 0 ; i < params.size() ; ++i) {
		if (i % 2 == 0) {
			set_neighbors(out, radius, radius, params[i], i + 1);
		} else {
			set_neighbors_diag(out, radius, radius, params[i], i + 1);
		}
	}
}

size_t manhattan_dist(int i_1, int j_1, int i_2, int j_2) {
	return abs(i_2 - i_1) + abs(j_2 - j_1);
}
/*
char normalize(int n, int max) {
	return
}
*/
void propagate_chamfrein(ImagePGM& out, vector<int>& params, size_t radius) {
/*
	int max = 0;

	for (size_t i = 0; i < params.size(); ++i) {
		if (max < params[i])
			max = params[i];
	}

	max *= radius * radius;
	int x = radius + 10, y = radius + 5;

	cerr << "Manhattan(" << radius << ", " << radius << ", " << x << ", " << y << ") = " << manhattan_dist(radius, radius, x, y) << "\n";
*/

	for (size_t i = 0; i < out.height(); ++i)
	{
		for (size_t j = 0; j < out.width(); ++j){

			size_t dist = manhattan_dist(radius, radius, i, j);

			// for (size_t k = 0 ; k < params.size() ; ++k){
			for (int k = params.size() - 1 ; k >= 0 ; --k){
				if (dist % (k+1) == 0) {
					if (dist <= radius)
						out(i, j) = params[k] * dist;// * (1.0 / (k+1));
				}
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		cerr << "Usage : " << argv[0] << " <filename>\n";
		return 1;
	}

	vector<int> params = {1, 1};
	size_t radius = 10;

	ImagePGM out;

	init_chamfrein(out, params, radius);
	propagate_chamfrein(out, params, radius);


	for (size_t i = 0; i < out.height(); ++i)
	{
		for (size_t j = 0; j < out.width(); ++j){
			cerr << (int)out(i, j) << "\t" ;
		}
		cerr <<"\n";
	}

	cout << out;

	return 0;
}

