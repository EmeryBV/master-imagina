#include <iostream>

#include "../PortableImageUtils.hpp"

using namespace std;

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
		PGM image_in(image_name);

		PGM image_out = image_in;

		cerr << "PSNR(Image_in, Image_out) = " << PSNR(image_in, image_out) << "\n";

		cerr << "Writing PGM...\n";
		cout << image_in;
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

		cerr << "PSNR(Image_in, Image_out) = " << PSNR(image_in, image_out) << "\n";

		cerr << "Writing PPM...\n";
		cout << image_out;
	}
	else
	{
		cerr << "Error : wrong image format (only PGM/PPM images supported)\n";
		exit(EXIT_SUCCESS);
	}

	return 0;
} 
