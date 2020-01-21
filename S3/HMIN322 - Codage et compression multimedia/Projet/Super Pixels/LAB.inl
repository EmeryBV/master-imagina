#ifndef COLOR_LAB_INL
#define COLOR_LAB_INL

#include "LAB.hpp"

template<typename T>
T& GenericLAB<T>::l()
{
	return (*this)[0];
}

template<typename T>
const T& GenericLAB<T>::l() const
{
	return (*this)[0];
}

template<typename T>
T& GenericLAB<T>::a()
{
	return (*this)[1];
}

template<typename T>
const T& GenericLAB<T>::a() const
{
	return (*this)[1];
}

template<typename T>
T& GenericLAB<T>::b()
{
	return (*this)[2];
}

template<typename T>
const T& GenericLAB<T>::b() const
{
	return (*this)[2];
}

#endif