#ifndef PORTABLE_IMAGE_UTILS_HPP
#define PORTABLE_IMAGE_UTILS_HPP

#include <limits>
#include "PortableImage.hpp"

/*
double EQM(const PGM& left, const PGM& right);
double PSNR(const PGM& left, const PGM& right);

double EQM(const PPM& left, const PPM& right);
double PSNR(const PPM& left, const PPM& right);

template<typename T>
T clamp(const T& value, const T& min = std::numeric_limits<T>::min(), const T& max = std::numeric_limits<T>::max());

double normalize(double n, double min, double max);

// Converti l'image RGB en YCbCr : R = Cr, G = Y, B = Cb

void RGB_to_YCbCr(PPM& img_rgb);
void YCbCr_to_RGB(PPM& img_ycbcr);
*/

double EQM(const PGM& left, const PGM& right)
{
	assert(left.same_dimension(right));

	double sum = 0;

	for (size_t i = 0 ; i < left.size() ; ++i)
	{
		sum += pow(left[i] - right[i], 2);
	}

	return sum / (double)left.size();
}

double PSNR(const PGM& left, const PGM& right)
{
	return 10 * log10(pow(255, 2) / EQM(left, right));
}

double EQM(const PPM& left, const PPM& right)
{
	assert(left.same_dimension(right));

	double eqm_r = 0;
	double eqm_g = 0;
	double eqm_b = 0;

	for (size_t i = 0 ; i < left.size() ; ++i)
	{
		eqm_r += pow(left[i].r - right[i].r, 2);
		eqm_g += pow(left[i].g - right[i].g, 2);
		eqm_b += pow(left[i].b - right[i].b, 2);
	}

	return (eqm_r + eqm_g + eqm_b) / (left.size() * 3);
}

double PSNR(const PPM& left, const PPM& right)
{
	return 10 * log10(pow(255, 2) / EQM(left, right));
}

template<typename T>
T clamp(const T& value, const T& min = std::numeric_limits<T>::min(), const T& max = std::numeric_limits<T>::max())
{
	assert(min < max);
	return value < min ? min : value > max ? max : value; 
}

double normalize(double n, double min, double max)
{
	return  (n - min) / (max - min);
}

void RGB_to_YCbCr(PPM& img_rgb)
{
	for (RGB& px : img_rgb)
	{
		double Y  =   0.299    * double(px.r) + 0.587    * double(px.g) + 0.114    * double(px.b);
		double Cb = - 0.168736 * double(px.r) - 0.331264 * double(px.g) + 0.5      * double(px.b) + 128;
		double Cr =   0.5      * double(px.r) - 0.418688 * double(px.g) - 0.081312 * double(px.b) + 128;
	
		px.g = round(Y);
		px.b = round(Cb);
		px.r = round(Cr);
	}
}

void YCbCr_to_RGB(PPM& img_ycbcr)
{
	for (RGB& px : img_ycbcr)
	{
		double Y  = px.g;
		double Cb = px.b;
		double Cr = px.r;

		px.r = clamp(Y + 1.402    * (Cr - 128));
		px.g = clamp(Y - 0.344136 * (Cb - 128) - 0.714136 * (Cr - 128));
		px.b = clamp(Y + 1.772    * (Cb - 128));
	}
}

#endif
