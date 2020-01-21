#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <fstream>
#include <initializer_list>
#include <cassert>

template<class T>
class Matrix
{
public:

	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = T*;
  	using const_iterator = T const*;
	
  	// Constructeurs

	Matrix();
	Matrix(size_t dim);
	Matrix(size_t rows, size_t cols);
	Matrix(size_t rows, size_t cols, const T& init_val);
	Matrix(std::initializer_list< std::initializer_list<T> > lists);

	// Cast operations
	
	template <typename U>
	explicit Matrix(const Matrix<U>& m);

	template<class U>
	explicit operator Matrix<U>() const;

	void resize(size_t rows, size_t cols);

	// Accessors

	size_t width()  const;
	size_t height() const;
	size_t size()   const;

	reference       operator[](size_t i);
	const_reference operator[](size_t i) const;

	reference       operator()(size_t row, size_t col);
	const_reference operator()(size_t row, size_t col) const;

	pointer       data();
	const_pointer data() const;

	// Iterators
	
	iterator       begin();
	const_iterator begin() const;

	iterator       end();
	const_iterator end() const;

	// Booleans
	
	bool on_bounds(size_t row, size_t col) const;
	bool same_dimension(const Matrix &right) const;

	// Matrix Operators

	Matrix operator+(const Matrix& m) const;
	Matrix operator-(const Matrix& m) const;
	Matrix operator*(const Matrix& m) const;

	Matrix& operator+=(const Matrix& m);
	Matrix& operator-=(const Matrix& m);
	Matrix& operator*=(const Matrix& m);

	// Scalar Operators
	
	template<class U>
	Matrix& operator+=(const U& value);
	template<class U>
	Matrix& operator-=(const U& value);
	template<class U>
	Matrix& operator*=(const U& value);
	template<class U>
	Matrix& operator/=(const U& value);

	template<class U, class V>
	friend Matrix<U> operator+(const Matrix<U>& m, const V& value);

	template<class U, class V>
	friend Matrix<U> operator-(const Matrix<U>& m, const V& value);
	
	template<class U, class V>
	friend Matrix<U> operator*(const Matrix<U>& m, const V& value);
	
	template<class U, class V>
	friend Matrix<U> operator/(const Matrix<U>& m, const V& value);

	// Boolean Operators

	template<class U>
	friend bool operator==(const Matrix<U>& left, const Matrix<U>& right);

	template<class U>
	friend bool operator!=(const Matrix<U>& left, const Matrix<U>& right);

	// Stream Operators

	template<typename U> 
	friend std::istream& operator>>(std::istream& is, Matrix<U>& m);

	template<typename U> 
	friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& m);

protected:

	size_t m_width;
	std::vector<T> m_data;
};

#include "Matrix.tpp"

#endif