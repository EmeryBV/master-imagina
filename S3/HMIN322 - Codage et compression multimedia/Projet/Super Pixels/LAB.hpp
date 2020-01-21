#ifndef COLOR_LAB_HPP
#define COLOR_LAB_HPP

#include "ScalarArray.hpp"

template<typename T>
struct GenericLAB : ScalarArray<T, 3>
{
	// Constructors

	using ScalarArray<T, 3>::ScalarArray;

	// Accessors

	      T& l();
	const T& l() const;

	      T& a();
	const T& a() const;

	      T& b();
	const T& b() const;
};

#include "LAB.inl"

using LAB  = GenericLAB<double>;


#endif