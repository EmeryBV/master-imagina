#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include "../lib/Image.h"
// #include "Image.h"

using namespace std;

double color_distance(const RGB& c1, const RGB& c2)
{
	return sqrt(pow(c1.r - c2.r, 2) + pow(c1.g - c2.g, 2) + pow(c1.b - c2.b, 2));
}

template<typename T>
T average(const vector<T>& v)
{
	T sum;
	for (const T& val : v)
		sum += val;

	return sum / v.size();
}

template<typename T>
void print(const vector<T>& v)
{
	for (const T& val : v)
	{
		cerr << val << "\n";
	}
}

double EQM(const ImagePPM& left, const ImagePPM& right)
{
	if (!left.same_dimension(right))
	{
		cerr << "Error : Image of different dimension\n";
		exit(EXIT_FAILURE);
	}

	double eqm_r = 0;
	double eqm_g = 0;
	double eqm_b = 0;

	for (size_t px = 0 ; px < left.size() ; px++)
	{
		eqm_r += pow(left[px].r - right[px].r, 2);
		eqm_g += pow(left[px].g - right[px].g, 2);
		eqm_b += pow(left[px].b - right[px].b, 2);
	}

	return (eqm_r + eqm_g + eqm_b) / (left.size() * 3);
}

double PSNR(const ImagePPM& left, const ImagePPM& right)
{
	return 10 * log10(pow(255, 2) / EQM(left, right));
}

ImagePGM reduced_by_2(const ImagePGM& img)
{
	ImagePGM reduced(img.width() / 2, img.height() / 2);

	for (size_t i = 0 ; i < reduced.height() ; ++i)
	{
		for (size_t j = 0 ; j < reduced.width() ; ++j)
		{
			reduced(i, j) = img(i * 2, j * 2);
		}
	}
	return std::move(reduced);
}

ImagePGM extended_by_2(const ImagePGM& img)
{
	ImagePGM extended(img.width() * 2, img.height() * 2);

	for (size_t i = 0 ; i < extended.height() ; ++i)
	{
		for (size_t j = 0 ; j < extended.width() ; ++j)
		{
			extended(i, j) = img(i / 2, j / 2);
		}
	}

	return std::move(extended);
}

// Converti l'image RGB en YCrCb : R = Cr, G = Y, B = Cb
ImagePPM RGB_to_YCbCr(const ImagePPM& img_rgb)
{
	ImagePPM YCrCb(img_rgb.width(), img_rgb.height());

	for (size_t i = 0 ; i < img_rgb.size() ; ++i)
	{
		YCrCb[i].g = 0.299		* img_rgb[i].r + 0.587	* img_rgb[i].g + 0.114		* img_rgb[i].b;
		YCrCb[i].b = -0.1687	* img_rgb[i].r - 0.3313	* img_rgb[i].g + 0.500		* img_rgb[i].b + 128;
		YCrCb[i].r = 0.500		* img_rgb[i].r - 0.4187	* img_rgb[i].g - 0.0813		* img_rgb[i].b + 128;
	}

	return std::move(YCrCb);
}

ImagePPM YCbCr_to_RGB(const ImagePPM& img_YCrCb)
{
	ImagePPM rgb(img_YCrCb.width(), img_YCrCb.height());

	for (size_t i = 0 ; i < img_YCrCb.size() ; ++i)
	{
		float Y = img_YCrCb[i].g;
		float Cb = img_YCrCb[i].b;
		float Cr = img_YCrCb[i].r;
		rgb[i].r = Y + 1.402   * (Cr - 128);
		rgb[i].g = Y - 0.34414 * (Cb - 128) - 071414 * (Cr - 128);
		rgb[i].b = Y + 1.772   * (Cb - 128);
	}

	return std::move(rgb);
}

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

	// Creation de l'image de sortie

	cerr << "Compression dimensionnelle de l'image RGB...\n";

	ImagePPM image_rgb_out(
		extended_by_2(reduced_by_2(image_in.red())),
		image_in.green(), 
		extended_by_2(reduced_by_2(image_in.blue()))
	);

	cerr << "PSNR(Image_in, Compressed_RGB) = " << PSNR(image_in, image_rgb_out) << "\n";

	image_rgb_out.save("rgb.ppm");

	cerr << "Compression dimensionnelle de l'image YCrCb...\n";

	ImagePPM image_YCbCr_in = RGB_to_YCbCr(image_in);

	ImagePPM image_YCbCr_out(
		extended_by_2(reduced_by_2(image_YCbCr_in.red())),
		image_YCbCr_in.green(), 
		extended_by_2(reduced_by_2(image_YCbCr_in.blue()))
	);

	image_YCbCr_out = YCbCr_to_RGB(image_YCbCr_out);

	cerr << "PSNR(Image_in, Compressed_YCbCr) = " << PSNR(image_in, image_YCbCr_out) << "\n";

	image_YCbCr_out.save("YCbCr.ppm");
	// cerr << "Ecriture de l'image resultante...\n";

	cout << image_rgb_out;

	return 0;
}
