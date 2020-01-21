#ifndef PORTABLE_IMAGE_UTILS_HPP
#define PORTABLE_IMAGE_UTILS_HPP

#include <limits>
#include "PortableImage.hpp"


double EQM(const PGM& left, const PGM& right);
double PSNR(const PGM& left, const PGM& right);

double EQM(const PPM& left, const PPM& right);
double PSNR(const PPM& left, const PPM& right);

template<typename T>
T clamp(const T& value, const T& min, const T& max);

double normalize(double n, double min, double max);

// Converti l'image RGB en YCbCr : R = Cr, G = Y, B = Cb

void RGB_to_YCbCr(PPM& img_rgb);
void YCbCr_to_RGB(PPM& img_ycbcr);

// Définition



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
		eqm_r += pow(left[i].r() - right[i].r(), 2);
		eqm_g += pow(left[i].g() - right[i].g(), 2);
		eqm_b += pow(left[i].b() - right[i].b(), 2);
	}

	return (eqm_r + eqm_g + eqm_b) / (left.size() * 3);
}

double PSNR(const PPM& left, const PPM& right)
{
	return 10 * log10(pow(255, 2) / EQM(left, right));
}

template<typename T>
T clamp(const T& value, const T& min, const T& max)
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
		double Y  =   0.299    * px.r() + 0.587    * px.g() + 0.114    * px.b();
		double Cb = - 0.168736 * px.r() - 0.331264 * px.g() + 0.5      * px.b() + 128;
		double Cr =   0.5      * px.r() - 0.418688 * px.g() - 0.081312 * px.b() + 128;
	
		px.g() = round(Y);
		px.b() = round(Cb);
		px.r() = round(Cr);
	}
}

void YCbCr_to_RGB(PPM& img_ycbcr)
{
	for (RGB& px : img_ycbcr)
	{
		double R = px.g() + 1.402    * (px.r() - 128);
		double G = px.g() - 0.344136 * (px.b() - 128) - 0.714136 * (px.r() - 128);
		double B = px.g() + 1.772    * (px.b() - 128);

		px.r() = clamp(round(R), 0.0, 255.0);
		px.g() = clamp(round(G), 0.0, 255.0);
		px.b() = clamp(round(B), 0.0, 255.0);
	}
}

#endif
