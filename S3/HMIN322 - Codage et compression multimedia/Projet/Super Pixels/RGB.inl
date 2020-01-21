#ifndef COLOR_RGB_INL
#define COLOR_RGB_INL

#include "RGB.hpp"

template<typename T>
T& GenericRGB<T>::r()
{
	return (*this)[0];
}

template<typename T>
const T& GenericRGB<T>::r() const
{
	return (*this)[0];
}

template<typename T>
T& GenericRGB<T>::g()
{
	return (*this)[1];
}

template<typename T>
const T& GenericRGB<T>::g() const
{
	return (*this)[1];
}

template<typename T>
T& GenericRGB<T>::b()
{
	return (*this)[2];
}

template<typename T>
const T& GenericRGB<T>::b() const
{
	return (*this)[2];
}

#endif