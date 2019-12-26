#include <iostream>
#include <array>

#include "PortableImageUtils.hpp"

using namespace std;

// PGM transform_ondelettes(const PGM& image)
// {
// 	PGM out(image.width(), image.height());

// 	for (size_t i = 0; i < image.height(); i+=2)
// 	{
// 		for (size_t j = 0; j < image.width(); j+=2)
// 		{
// 			double A = image(i, j);
// 			double B = image(i, j+1);
// 			double C = image(i+1, j);
// 			double D = image(i+1, j+1);

// 			double X = double(A + B + C + D) / 4.0;
// 			double Y = double(A + B - C - D) / 2.0;
// 			double Z = double(A - B + C - D) / 2.0;
// 			double O = double(A - B - C + D);

// 			out((i / 2), (j / 2)) = X;
// 			out((i / 2), image.width() / 2 + (j / 2)) = Y;
// 			out(image.height() / 2 + (i / 2), (j / 2)) = Z;
// 			out(image.height() / 2 + (i / 2), image.width() / 2 + (j / 2)) = O;
// 		}
// 	}

// 	return out;
// }

PGM reduced_by_2(const PGM& img)
{
	PGM reduced(img.width() / 2, img.height() / 2);

	for (size_t i = 0 ; i < reduced.height() ; ++i)
	{
		for (size_t j = 0 ; j < reduced.width() ; ++j)
		{	
			reduced(i, j) = img(i * 2, j * 2);
		}
	}

	return reduced;
}

PGM extended_by_2(const PGM& img)
{
	PGM extended(img.width() * 2, img.height() * 2);

	// Etant le pixel comme un block.

	for (size_t i = 0 ; i < extended.height() ; i++)
		for (size_t j = 0  ; j < extended.width() ; j++)
			extended(i, j) = img(i / 2, j / 2);

	// Moyenne des 4 pixels voisins de img

	for (size_t i = 0 ; i < img.height() ; ++i)
	{
		for (size_t j = 0 ; j < img.width() ; ++j)
		{
			std::array<PGM::value_type, 4> pixels = {
				img(i    , j),
				img(i    , j + 1),
				img(i + 1, j),
				img(i + 1, j + 1),
			};

			double mean = 0;

			for (const auto& pixel : pixels)
				mean += pixel;

			mean /= pixels.size();

			extended((i * 2) + 1, (j * 2) + 1) = round(mean);
		}
	}

	// moyenne des pixels voisins

	for (size_t i = 1 ; i < extended.height() - 1 ; i++)
	{
		for (size_t j = (i % 2 == 0 ? 1 : 2)  ; j < extended.width() - 1 ; j+=2)
		{
			std::array<PGM::value_type, 4> pixels = {
				extended(i - 1, j),
				extended(i    , j + 1),
				extended(i + 1, j),
				extended(i    , j - 1)
			};

			double mean = 0;

			for (const auto& pixel : pixels)
				mean += pixel;

			mean /= pixels.size();
			
			extended(i, j) = round(mean);
		}
	}
	
	return extended;
}

PPM reduced_by_2(const PPM& img)
{
	return PPM(
		reduced_by_2(img.red()),
		reduced_by_2(img.green()),
		reduced_by_2(img.blue())
		);
}

PPM extended_by_2(const PPM& img)
{
	return PPM(
		extended_by_2(img.red()),
		extended_by_2(img.green()),
		extended_by_2(img.blue())
		);
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
    {
    	cerr << "Usage : " << argv[0] << " <image.ppm/pgm>\n";
		exit(EXIT_FAILURE);
	}

	string image_name = argv[1];

	PortableImageHeader header; header.load(image_name);

	if (is_pgm_header(header))
	{
		cerr << "Reading PGM...\n";
		// PGM image_in(image_name);

		// PGM image_out = reduced_by_2(image_in);

		// cerr << "Taille originale    : " << image_in.size()  << " octets\n";
		// cerr << "Taille compressée   : " << image_out.size() << " octets\n";
		// cerr << "Taux de compression : " << (double)image_in.size() / (double)image_out.size() << '\n';
		// cerr << "PSNR : " << PSNR(image_in, extended_by_2(image_out)) << '\n';

		// cerr << "Writing PGM...\n";
		// cout << extended_by_2(image_out);
		// cout << image_out;
	}
	else if (is_ppm_header(header))
	{
		cerr << "Reading PPM...\n";
		PPM image_in(image_name);

		PPM image_out = image_in;

		cerr << "Transformation spacial (RGB -> YCbCr)...\n";
		RGB_to_YCbCr(image_out);

		cerr << "Transformation spacial (YCbCr -> RGB)...\n";
		YCbCr_to_RGB(image_out);

		cerr << "PSNR : " << PSNR(image_in, image_out) << '\n';

		cerr << "Writing PPM...\n";
		// cout << extended_by_2(image_out);
		// cout << image_out;
	}
	else
	{
		cerr << "Error : wrong image format (only PGM/PPM images supported)\n";
		exit(EXIT_SUCCESS);
	}

	return 0;
} 
