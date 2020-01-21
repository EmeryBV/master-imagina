#ifndef SCALAR_ARRAY_HPP
#define SCALAR_ARRAY_HPP

#include <array>
#include <fstream>


///////////////////////////////////////////////// SCALAR_ARRAY


template<class T, size_t N>
struct ScalarArray : public std::array<T, N>
{
	// -- Constructors --
	
	// Méthodes d'initialisation possibles :
	// 
	// ScalarArray<T, N> s_array;                     -> Contenu du tableau initialisé à zero.
	// ScalarArray<T, N> s_array(v_0, v_1, ..., v_n); -> Initialisation de chacune des valeurs du tableau.
	// ScalarArray<T, N> s_array{v_0, v_1, v_3, ...}; -> Initialisation des valeurs celles qui ne sont pas initialisées sont mise à zero.
	
	using std::array<T, N>::array;

	ScalarArray();
	
	template <typename... Types>
	ScalarArray(const Types&... ts);

	ScalarArray(std::initializer_list<T> init);

	template<typename U> // explicit cast constructor  
	explicit ScalarArray(const ScalarArray<U, N>& s_array);

	// -- Unary arithmetic operators --

	ScalarArray          operator+() const;
	ScalarArray          operator-() const;	
	ScalarArray          operator~() const; // Integral only
	ScalarArray<bool, N> operator!() const;

	// -- Arithmetic assignement operators --

	ScalarArray& operator+= (const ScalarArray& s_array);
	ScalarArray& operator-= (const ScalarArray& s_array);
	ScalarArray& operator*= (const ScalarArray& s_array);
	ScalarArray& operator/= (const ScalarArray& s_array);

	ScalarArray& operator+= (const T& value);
	ScalarArray& operator-= (const T& value);
	ScalarArray& operator*= (const T& value);
	ScalarArray& operator/= (const T& value);

	// -- Bit arithmetic assignement operators --

	ScalarArray& operator%= (const ScalarArray& s_array); // Integral only
	ScalarArray& operator&= (const ScalarArray& s_array); // Integral only
	ScalarArray& operator|= (const ScalarArray& s_array); // Integral only
	ScalarArray& operator^= (const ScalarArray& s_array); // Integral only
	ScalarArray& operator<<=(const ScalarArray& s_array); // Integral only
	ScalarArray& operator>>=(const ScalarArray& s_array); // Integral only

	ScalarArray& operator%= (const T& value); // Integral only
	ScalarArray& operator&= (const T& value); // Integral only
	ScalarArray& operator|= (const T& value); // Integral only
	ScalarArray& operator^= (const T& value); // Integral only
	ScalarArray& operator<<=(const T& value); // Integral only
	ScalarArray& operator>>=(const T& value); // Integral only
};


///////////////////////////////////////////////// SCALAR_ARRAY NON-MEMBER FUNCTIONS

// -- Arithmetic assignement operators --

template <class T, size_t N>
ScalarArray<T, N>    operator+ (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator- (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator* (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator/ (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);

template <class T, size_t N>
ScalarArray<T, N>    operator+ (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator- (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator* (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator/ (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);

template <class T, size_t N>
ScalarArray<T, N>    operator+ (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator- (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator* (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator/ (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);

// -- Bit arithmetic assignement operators --

template <class T, size_t N>   
ScalarArray<T, N>    operator% (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator& (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator| (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator^ (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator<<(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator>>(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right); // Integral only

template <class T, size_t N>   
ScalarArray<T, N>    operator% (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator& (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator| (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator^ (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator<<(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator>>(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value); // Integral only

template <class T, size_t N>   
ScalarArray<T, N>    operator% (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator& (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator| (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator^ (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator<<(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array); // Integral only
template <class T, size_t N>   
ScalarArray<T, N>    operator>>(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array); // Integral only

// -- Boolean arithmetic assignement operators --

template <class T, size_t N>
ScalarArray<bool, N> operator&&(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>
ScalarArray<bool, N> operator||(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);

template <class T, size_t N>
ScalarArray<bool, N> operator&&(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>
ScalarArray<bool, N> operator||(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);

template <class T, size_t N>
ScalarArray<bool, N> operator&&(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>
ScalarArray<bool, N> operator||(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);

// -- Stream operators --

template <class T, size_t N>
std::ostream& operator<<(std::ostream& os, const ScalarArray<T, N>& s_array);
template <class T, size_t N>
std::istream& operator>>(std::istream& is, const ScalarArray<T, N>& s_array);

#include "ScalarArray.inl"

#endif