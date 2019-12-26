// #include "ScalarArray.hpp"


///////////////////////////////////////////////// SCALAR_ARRAY


//// Constructors


template<class T, size_t N>
template<class... Args>
ScalarArray<T, N>::ScalarArray(const T& arg, const Args&... args) : std::array<T, N>{arg, args...}
{
    static_assert(sizeof...(Args) + 1 == N, "wrong constructor arity");
}


//// Unary arithmetic operator+,-,~,!


template<class T, size_t N>
ScalarArray<T, N> ScalarArray<T, N>::operator+() const
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = +(*this)[i];

	return result;
}

template<class T, size_t N>
ScalarArray<T, N> ScalarArray<T, N>::operator-() const
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = -(*this)[i];

	return result;
}

template<class T, size_t N>
ScalarArray<T, N> ScalarArray<T, N>::operator~() const
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = ~(*this)[i];

	return result;
}

template<class T, size_t N>
ScalarArray<bool, N> ScalarArray<T, N>::operator!() const
{
	ScalarArray<bool, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = !(*this)[i];

	return result;
}


//// Compound assignment operator+,-,*,/,%,&,|,^,<<,>>,&&,||


template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator+= (const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] += s_array[i];

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator-= (const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] -= s_array[i];

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator*= (const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] *= s_array[i];

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator/= (const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] /= s_array[i];

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator%= (const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] %= s_array[i];

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator&= (const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] &= s_array[i];

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator|= (const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] |= s_array[i];

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator^= (const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] ^= s_array[i];

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator<<=(const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] <<= s_array[i];

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator>>=(const ScalarArray& s_array)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] >>= s_array[i];

	return (*this);
}


//// Compound assignment operator+,-,*,/,%,&,|,^,<<,>>,&&,||


template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator+= (const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] += value;

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator-= (const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] -= value;

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator*= (const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] *= value;

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator/= (const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] /= value;

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator%= (const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] %= value;

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator&= (const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] &= value;

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator|= (const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] |= value;

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator^= (const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] ^= value;

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator<<=(const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] <<= value;

	return (*this);
}

template<class T, size_t N>
ScalarArray<T, N>& ScalarArray<T, N>::operator>>=(const T& value)
{
	for (size_t i = 0 ; i < this->size() ; i++)
		(*this)[i] >>= value;

	return (*this);
}


///////////////////////////////////////////////////////////////////////////////////////////////// SCALAR_ARRAY NON-MEMBER FUNCTIONS


//// Binary operator+,-,*,/,%,&,|,^,<<,>>,&&,||


template <class T, size_t N>
ScalarArray<T, N>    operator+ (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] + right[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator- (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] - right[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator* (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] * right[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator/ (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] / right[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator% (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] % right[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator& (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] & right[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator| (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] | right[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator^ (const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] ^ right[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator<<(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] << right[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator>>(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] >> right[i];

	return result;
}

template <class T, size_t N>
ScalarArray<bool, N> operator&&(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<bool, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] && right[i];

	return result;
}

template <class T, size_t N>
ScalarArray<bool, N> operator||(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	ScalarArray<bool, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = left[i] || right[i];

	return result;
}


//// Binary operator+,-,*,/,%,&,|,^,<<,>>,&&,||


template <class T, size_t N>
ScalarArray<T, N>    operator+ (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] + value;

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator- (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] - value;

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator* (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] * value;

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator/ (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] / value;

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator% (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] % value;

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator& (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] & value;

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator| (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] | value;

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator^ (const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] ^ value;

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator<<(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] << value;

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator>>(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] >> value;

	return result;
}

template <class T, size_t N>
ScalarArray<bool, N> operator&&(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<bool, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] && value;

	return result;
}

template <class T, size_t N>
ScalarArray<bool, N> operator||(const ScalarArray<T, N>& s_array, const typename ScalarArray<T, N>::value_type& value)
{
	ScalarArray<bool, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = s_array[i] || value;

	return result;
}


//// Binary operator+,-,*,/,%,&,|,^,<<,>>,&&,||


template <class T, size_t N>
ScalarArray<T, N>    operator+ (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value + s_array[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator- (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value - s_array[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator* (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value * s_array[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator/ (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value / s_array[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator% (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value % s_array[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator& (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value & s_array[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator| (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value | s_array[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator^ (const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value ^ s_array[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator<<(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value << s_array[i];

	return result;
}

template <class T, size_t N>   
ScalarArray<T, N>    operator>>(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<T, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value >> s_array[i];

	return result;
}

template <class T, size_t N>
ScalarArray<bool, N> operator&&(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<bool, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value && s_array[i];

	return result;
}

template <class T, size_t N>
ScalarArray<bool, N> operator||(const typename ScalarArray<T, N>::value_type& value, const ScalarArray<T, N>& s_array)
{
	ScalarArray<bool, N> result;

	for (size_t i = 0 ; i < result.size() ; i++)
		result[i] = value || s_array[i];

	return result;
}


//// Stream operator<<,>>


template <class T, size_t N>
std::ostream& operator<<(std::ostream& os, const ScalarArray<T, N>& s_array)
{
	os << '(' << s_array.front();

	for (size_t i = 1 ; i < s_array.size() ; i++) 
		os << ", " << s_array[i];

	return os << ')';
}

template <class T, size_t N>
std::istream& operator>>(std::istream& is, const ScalarArray<T, N>& s_array)
{
	for (auto& s : s_array)
		is >> s;

	return is;
}
