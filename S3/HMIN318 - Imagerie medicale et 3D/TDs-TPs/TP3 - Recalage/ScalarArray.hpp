#ifndef SCALAR_ARRAY_HPP
#define SCALAR_ARRAY_HPP

#include <array>
#include <fstream>


///////////////////////////////////////////////// SCALAR_ARRAY


template<class T, size_t N>
struct ScalarArray : public std::array<T, N>
{
	//// Constructors
	
	
	// Méthodes d'initialisation possibles :
	// 
	// ScalarArray<T, N> s_array;   -> ATTENTION : Contenu du tableau indéfinie 
	// ScalarArray<T, N> s_array{}; -> Contenu du tableau initialisé à zero 
	// 
	// ScalarArray<T, N> s_array; std::fill(s_array.begin(), s_array.end(), T()); -> Initialisation du tableau avec une unique valeur
	// 
	// ScalarArray<T, N> s_array{v_0, v_1, ..., v_n}; -> Initialisation de chacune des valeurs du tableau
	// ScalarArray<T, N> s_array(v_0, v_1, ..., v_n); -> Initialisation de chacune des valeurs du tableau

	using std::array<T, N>::array;

    template<class... Args>
	ScalarArray(const T& arg, const Args&... args);


	//// Unary arithmetic operator+,-,~,!


	ScalarArray          operator+() const;
	ScalarArray          operator-() const;
	ScalarArray          operator~() const;
	ScalarArray<bool, N> operator!() const;


	//// Compound assignment operator+,-,*,/,%,&,|,^,<<,>>,&&,||


	ScalarArray& operator+= (const ScalarArray& s_array);
	ScalarArray& operator-= (const ScalarArray& s_array);
	ScalarArray& operator*= (const ScalarArray& s_array);
	ScalarArray& operator/= (const ScalarArray& s_array);
	ScalarArray& operator%= (const ScalarArray& s_array);
	ScalarArray& operator&= (const ScalarArray& s_array);
	ScalarArray& operator|= (const ScalarArray& s_array);
	ScalarArray& operator^= (const ScalarArray& s_array);
	ScalarArray& operator<<=(const ScalarArray& s_array);
	ScalarArray& operator>>=(const ScalarArray& s_array);


	//// Compound assignment operator+,-,*,/,%,&,|,^,<<,>>,&&,||


	ScalarArray& operator+= (const T& value);
	ScalarArray& operator-= (const T& value);
	ScalarArray& operator*= (const T& value);
	ScalarArray& operator/= (const T& value);
	ScalarArray& operator%= (const T& value);
	ScalarArray& operator&= (const T& value);
	ScalarArray& operator|= (const T& value);
	ScalarArray& operator^= (const T& value);
	ScalarArray& operator<<=(const T& value);
	ScalarArray& operator>>=(const T& value);
};


///////////////////////////////////////////////// SCALAR_ARRAY NON-MEMBER FUNCTIONS


//// Binary operator+,-,*,/,%,&,|,^,<<,>>,&&,||


template <class T, size_t N>
ScalarArray<T, N>    operator+ (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator- (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator* (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator/ (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator% (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator& (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator| (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator^ (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator<<(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>   
ScalarArray<T, N>    operator>>(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>
ScalarArray<bool, N> operator&&(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);
template <class T, size_t N>
ScalarArray<bool, N> operator||(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right);


//// Binary operator+,-,*,/,%,&,|,^,<<,>>,&&,||


template <class T, size_t N>
ScalarArray<T, N>    operator+ (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator- (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator* (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator/ (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator% (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator& (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator| (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator^ (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator<<(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>   
ScalarArray<T, N>    operator>>(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>
ScalarArray<bool, N> operator&&(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);
template <class T, size_t N>
ScalarArray<bool, N> operator||(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value);


//// Binary operator+,-,*,/,%,&,|,^,<<,>>,&&,||


template <class T, size_t N>
ScalarArray<T, N>    operator+ (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator- (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator* (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator/ (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator% (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator& (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator| (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator^ (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator<<(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>   
ScalarArray<T, N>    operator>>(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>
ScalarArray<bool, N> operator&&(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);
template <class T, size_t N>
ScalarArray<bool, N> operator||(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array);


//// Stream operator<<,>>


template <class T, size_t N>
std::ostream& operator<<(std::ostream& os, const ScalarArray<T, N>& s_array);
template <class T, size_t N>
std::istream& operator>>(std::istream& is, const ScalarArray<T, N>& s_array);


#include "ScalarArray.inl"


#endif