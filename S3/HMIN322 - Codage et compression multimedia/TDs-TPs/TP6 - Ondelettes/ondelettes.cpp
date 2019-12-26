#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <array>
#include "../lib/Image.h"
// #include "Image.h"

using namespace std;

// Converti l'image RGB en YCbCr : R = Cr, G = Y, B = Cb

void RGB_to_YCbCr(ImagePPM& img_rgb)
{
	for (RGB& px : img_rgb)
	{
		double Y  = 0.299		* double(px.r) + 0.587		* double(px.g) + 0.114		* double(px.b);
		double Cb =-0.168736	* double(px.r) - 0.331264	* double(px.g) + 0.5		* double(px.b) + 128;
		double Cr = 0.5		* double(px.r) - 0.418688	* double(px.g) - 0.081312	* double(px.b) + 128;
	
		px.g = round(Y);
		px.b = round(Cb);
		px.r = round(Cr);
	}
}

void YCbCr_to_RGB(ImagePPM& img_YCbCr)
{
	for (RGB& px : img_YCbCr)
	{
		double Y  = px.g;
		double Cb = px.b;
		double Cr = px.r;

		px.r = clamp(Y + 1.402		* (Cr - 128));
		px.g = clamp(Y - 0.344136	* (Cb - 128) - 0.714136 * (Cr - 128));
		px.b = clamp(Y + 1.772		* (Cb - 128));
	}
}

ImagePPM transform_ondelettes(const ImagePPM& image)
{
	ImagePPM out(image.width(), image.height());

	for (size_t i = 0; i < image.height(); i+=2)
	{
		for (size_t j = 0; j < image.width(); j+=2)
		{
			RGBd A = image(i, j);
			RGBd B = image(i, j+1);
			RGBd C = image(i+1, j);
			RGBd D = image(i+1, j+1);

			RGBd X = RGBd(A + B + C + D) / 4;
			RGBd Y = RGBd(A + B - C - D) / 2;
			RGBd Z = RGBd(A - B + C - D) / 2;
			RGBd O = RGBd(A - B - C + D);

			out((i / 2), (j / 2)) = X;
			out((i / 2), image.width() / 2 + (j / 2)) = Y;
			out(image.height() / 2 + (i / 2), (j / 2)) = Z;
			out(image.height() / 2 + (i / 2), image.width() / 2 + (j / 2)) = O;
		}
	}

	return std::move(out);
}
/*
vector<double> convolution_kernel =
{
	-1, -1, -1,
	-1, -8, -1,
	-1, -1, -1,
};

vector<double> nette =
{
	0, -1, 0,
	-1, 5, -1,
	0, -1, 0,
};

// la matrice doit etre carré
ImagePPM transform(const ImagePPM& image, const vector<double>& transform_matrix)
{	
	ImagePPM out = image;

	size_t transform_n 	= (size_t)sqrt(transform_matrix.size());
	size_t transform_offset = transform_n / 2;

	cerr << "matrix_n = " << transform_n << "\n";
	cerr << "matrix_offset = " << transform_offset << "\n";

	for (size_t i = transform_offset; i < image.height() - transform_offset; ++i)
	{
		for (size_t j = transform_offset; j < image.width() - transform_offset; ++j)
		{
			RGBd pixel_value;

			for (size_t T_i = 0 ; T_i < transform_n ; ++T_i)
			{
				for (size_t T_j = 0 ; T_j < transform_n ; ++T_j)
				{
					RGBd pixel = image((i - transform_offset) + T_i, (j - transform_offset) + T_j);
					pixel_value +=  pixel * transform_matrix[T_i * transform_n + T_j];
				}
			}

			out(i, j) = pixel_value;
		}
	}

	return std::move(out);
}
*/
int main(int argc, char** argv)
{
	if (argc < 2)
    {
    	cerr << "Usage : " << argv[0] << " <image_in.ppm>\n";
		return EXIT_FAILURE;
	}

	string image_name = argv[1];

	ImagePPM image_in;

	cerr << "Lecture de l'image...\n";

	image_in.load(image_name);

	ImagePPM image_out = image_in;
	
	cerr << "Transformation spacial (RGB -> YCbCr)...\n";

	// ImagePPM image_out = transform(image_in, convolution_kernel);

	RGB_to_YCbCr(image_out);

	image_out = transform_ondelettes(image_out);

	cerr << "Transformation spacial (YCbCr -> RGB)...\n";

	YCbCr_to_RGB(image_out);

	cerr << "PSNR(Image_in, Image_out) = " << PSNR(image_in, image_out) << "\n";

	cerr << "Ecriture de l'image...\n";

	cout << image_out;

	return 0;
}

/*

moyenne des moyennes -> basse fréquences

différences des moyennes -> fréquences horizontales

différences des différences -> très hautes fréquences

prendre puissance de deux pour question 2

appliquer quantification -> avoir un coef unique par sous bandes





codage huffman : tracer courbe y debit (bits/pixel), x distorsion (PSNR)



taux de compression rapport de tailles entre les images 

objectifs combien de décomposition dois-t-on faire ? tester les changements ?
 */