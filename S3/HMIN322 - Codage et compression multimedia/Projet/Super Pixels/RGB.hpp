#ifndef COLOR_RGB_HPP
#define COLOR_RGB_HPP

#include "ScalarArray.hpp"

template<typename T>
struct GenericRGB : ScalarArray<T, 3>
{
	// Constructors

	using ScalarArray<T, 3>::ScalarArray;

	// Accessors

	      T& r();
	const T& r() const;

	      T& g();
	const T& g() const;

	      T& b();
	const T& b() const;
};

#include "RGB.inl"

using RGB = GenericRGB<unsigned char>;
using RGBi = GenericRGB<unsigned int>;
using RGBf  = GenericRGB<float>;
using RGBd  = GenericRGB<double>;

#endif