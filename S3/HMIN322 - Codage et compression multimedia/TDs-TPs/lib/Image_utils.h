#ifndef PORTABLE_IMAGE_UTILS_HPP 
#define PORTABLE_IMAGE_UTILS_HPP 

#include "PortableImage.hpp"

double EQM(const ImagePGM& left, const ImagePGM& right);
double PSNR(const ImagePGM& left, const ImagePGM& right);

double EQM(const ImagePPM& left, const ImagePPM& right);
double PSNR(const ImagePPM& left, const ImagePPM& right);

template<typename T>
T clamp(const T& value, const T& min = std::numeric_limits<T>::min(), const T& max = std::numeric_limits<T>::max());

double normalize(double n, double min, double max);

// Converti l'image RGB en YCbCr : R = Cr, G = Y, B = Cb

void RGB_to_YCbCr(ImagePPM& img_rgb);
void YCbCr_to_RGB(ImagePPM& img_ycbcr);

#endif
