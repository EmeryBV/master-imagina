#ifndef DYNAMIC_MATRIX_INL
#define DYNAMIC_MATRIX_INL

#include "Matrix.hpp"

// Constructeurs

template<class T>
Matrix<T>::Matrix() : m_width(0), m_data() {}

template<class T>
Matrix<T>::Matrix(size_t dim) : m_width(dim), m_data(dim * dim) {}

template<class T>
Matrix<T>::Matrix(size_t rows, size_t cols) :  m_width(cols), m_data(rows * cols){}

template<class T>
Matrix<T>::Matrix(size_t rows, size_t cols, const T& init_val) : m_width(cols), m_data(rows * cols, init_val) {}

template<class T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> list) : Matrix(list.size(), list.begin()->size())
{
	size_t i = 0;

	for (const auto& rows : list)
	{
		assert(this->width() == rows.size());

		for (const auto& col : rows)
		{
			m_data[i] = col;
			i++;
		}
	}
}

// Cast operations

template<class T>
template<class U>
Matrix<T>::Matrix(const Matrix<U>& m) : m_width(m.width()), m_data(m.height() * m.width())
{
	for (size_t i = 0 ; i < m.size() ; ++i)
	{
		(*this)[i] = static_cast<T>(m[i]);
	}
}

template<class T>
template<class U>
Matrix<T>::operator Matrix<U>() const
{
	Matrix<U> result(this->height(), this->width());

	for (size_t i = 0 ; i < this->size() ; ++i)
	{
		result[i] = static_cast<U>((*this)[i]);
	}

	return result;
}

template<class T>
void Matrix<T>::resize(size_t rows, size_t cols)
{
	m_data.resize(rows * cols);
	m_width = cols;
}

// Accessors

template<class T>
size_t Matrix<T>::width() const  { return this->m_width; }

template<class T>
size_t Matrix<T>::height() const { return this->size() == 0 ? 0 : this->size() / this->width(); }

template<class T>
size_t Matrix<T>::size() const   { return this->m_data.size(); }


template<class T>
typename Matrix<T>::reference       Matrix<T>::operator[](size_t i)       { return this->m_data[i]; }
template<class T>
typename Matrix<T>::const_reference Matrix<T>::operator[](size_t i) const { return this->m_data[i]; }

template<class T>
typename Matrix<T>::reference       Matrix<T>::operator()(size_t row, size_t col)       { return this->m_data[row * this->width() + col]; }
template<class T>
typename Matrix<T>::const_reference Matrix<T>::operator()(size_t row, size_t col) const { return this->m_data[row * this->width() + col]; }

template<class T>
typename Matrix<T>::pointer       Matrix<T>::data()       { return this->m_data.data(); }
template<class T>
typename Matrix<T>::const_pointer Matrix<T>::data() const { return this->m_data.data(); }

// Iterators

template<class T>
typename Matrix<T>::iterator       Matrix<T>::begin()       { return this->data(); }
template<class T>
typename Matrix<T>::const_iterator Matrix<T>::begin() const { return this->data(); }


template<class T>
typename Matrix<T>::iterator       Matrix<T>::end()       { return this->data() + this->size(); }
template<class T>
typename Matrix<T>::const_iterator Matrix<T>::end() const { return this->data() + this->size(); }

// Static functions

template<class T>
bool Matrix<T>::on_bounds(size_t row, size_t col) const
{
	return row < this->height() && col < this->width();
}

template<class T>
bool Matrix<T>::same_dimension(const Matrix<T>& m) const
{
	return this->height() == m.height() && this->width() == m.width();
}

// Matrix Operators

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& m) const
{
	assert(same_dimension((*this), m));

	Matrix<T> result(m.height(), m.width());

	for (size_t i = 0; i < m.size(); ++i)
		result[i] = (*this)[i] + m[i];
	
	return result;
} 

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& m) const
{
	assert(same_dimension((*this), m));

	Matrix<T> result(m.height(), m.width());

	for (size_t i = 0; i < m.size(); ++i)
		result[i] = (*this)[i] - m[i];
	
	return result;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& m) const
{
	assert(this->width()  == m.height());

	Matrix<T> result(this->height(), m.width());

	for (size_t i = 0; i < result.height(); ++i)
	{
		for (size_t j = 0; j < result.width(); ++j)
		{
			T sum = 0;

			for (size_t k = 0; k < this->width(); ++k)
			{
				sum += (*this)(i, k) * m(k, j);
			}

			result(i, j) = sum;
		}
	}

	return result;
}


template<class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& m)
{
	assert(same_dimension((*this), m));

	for (size_t i = 0; i < this->size(); ++i)
		(*this)[i] += m[i];

	return (*this);
} 

template<class T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& m)
{
	assert(same_dimension((*this), m));

	for (size_t i = 0; i < this->size(); ++i)
		(*this)[i] -= m[i];

	return (*this);
}

template<class T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& m)
{
	assert(this->width() == m.height());

	Matrix<T> result(this->height(), m.width());

	for (size_t i = 0; i < result.height(); ++i)
	{
		for (size_t j = 0; j < result.width(); ++j)
		{
			T sum = 0;

			for (size_t k = 0; k < this->width(); ++k)
			{
				sum += (*this)(i, k) * m(k, j);
			}

			result(i, j) = sum;
		}
	}

	(*this) = result;

	return (*this);
}

// Scalar Operators

template<class T>
template<class U>
Matrix<T>& Matrix<T>::operator+=(const U& value)
{
	for (T& v : (*this))
		v += value;

	return (*this);
}

template<class T>
template<class U>
Matrix<T>& Matrix<T>::operator-=(const U& value)
{
	return (*this) += (-value);
}

template<class T>
template<class U>
Matrix<T>& Matrix<T>::operator*=(const U& value)
{
	for (T& v : (*this))
		v *= value;

	return (*this);
}

template<class T>
template<class U>
Matrix<T>& Matrix<T>::operator/=(const U& value)
{
	for (T& v : (*this))
		v /= value;

	return (*this);
}

template<class U, class V>
Matrix<U> operator+(const Matrix<U>& m, const V& value)
{
	Matrix<U> result(m.height(), m.width());

	for (size_t i = 0; i < m.size(); ++i)
		result[i] = m[i] + value;

	return result;
}

template<class U, class V>
Matrix<U> operator-(const Matrix<U>& m, const V& value)
{
	return m + (-value);
}

template<class U, class V>
Matrix<U> operator*(const Matrix<U>& m, const V& value)
{
	Matrix<U> result(m.height(), m.width());

	for (size_t i = 0; i < m.size(); ++i)
		result[i] = m[i] * value;
	
	return result;
}

template<class U, class V>
Matrix<U> operator/(const Matrix<U>& m, const V& value)
{
	Matrix<U> result(m.height(), m.width());

	for (size_t i = 0; i < m.size(); ++i)
		result[i] = m[i] / value;
	
	return result;
}

// Boolean Operators

template<class U>
bool operator==(const Matrix<U>& left, const Matrix<U>& right)
{
	assert(left.same_dimension(right));

	for (size_t i = 0; i < left.size(); ++i)
	{
		if (left[i] != right[i])
			return false;
	}
	
	return true;
}

template<class U>
bool operator!=(const Matrix<U>& left, const Matrix<U>& right)
{
	return !(left == right);
}

// Stream Operators

template<class T> 
std::istream& operator>>(std::istream& is, Matrix<T>& m)
{
	for (auto& v : m)
		is >> v;

	return is;
}

template<class T> 
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
	for (size_t i = 0 ; i < m.height() ; ++i)
	{
		for (size_t j = 0 ; j < m.width() ; ++j)
		{
			os << m(i, j) << "\t";
		}
		os << '\n';
	}

	return os;
}

#endif