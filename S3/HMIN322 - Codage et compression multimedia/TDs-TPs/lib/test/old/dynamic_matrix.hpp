#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <vector>

template<class T>
class Dynamic_matrix
{
public:
	Dynamic_matrix() : {}
	Dynamic_matrix(size_t rows, size_t cols) : {}
	Dynamic_matrix(size_t rows, size_t cols, const T& init_val) : {}
private:
	size_t m_width;
	std::vector<T> m_data;
};

#endif

template<typename T>
class ImageBase
{
public:

	using iterator = T*;
  	using const_iterator = T const*;

	// Constructors

	ImageBase()								: m_width(0), m_height(0), m_data() {}
	ImageBase(size_t width, size_t height)	: m_width(width), m_height(height), m_data(m_width * m_height) {}
	ImageBase(size_t width, size_t height, const T& value)	: m_width(width), m_height(height), m_data(m_width * m_height, value) {}
	
	void resize(size_t width, size_t height);

	// Accessors

	size_t width() const 	{ return this->m_width; }
	size_t height() const 	{ return this->m_height; }
	size_t size() const  	{ return this->m_data.size(); }

	T& operator[](size_t n) 				{ return this->m_data[n]; }
	const T& operator[](size_t n) const 	{ return this->m_data[n]; }

	// Matrix style acess : data(i, j) =
	T& operator()(size_t row, size_t column) 				{ return this->m_data[row * this->width() + column]; }
	const T& operator()(size_t row, size_t column) const 	{ return this->m_data[row * this->width() + column]; }

	T* data() 				{ return this->m_data.data(); }
	const T* data() const 	{ return this->m_data.data(); }

	iterator begin() 				{ return this->data(); }
	const_iterator begin() const 	{ return this->data(); }

	iterator end() 				{ return this->data() + this->size(); }
	const_iterator end() const 	{ return this->data() + this->size(); }

	// Check functions

	bool on_bounds(size_t row, size_t column) const 	{ return row < this->height() && column < this->width(); }
	bool same_dimension(const ImageBase& image) const 	{ return this->height() == image.height() && this->width() == image.width(); }

	// Read/Write functions
	
	void load(const std::string& filename);
	void save(const std::string& filename);

	template<typename U> 
	friend std::istream& operator>>(std::istream& is, ImageBase<U>& image);
	template<typename U> 
	friend std::ostream& operator<<(std::ostream& os, const ImageBase<U>& image);

protected:

	size_t m_width;
	size_t m_height;
	std::vector<T> m_data;

	// charge une image en fonction de sont type (.ppm / .pgm)

	virtual void read(std::istream& is) = 0;
	virtual void write(std::ostream& os) const = 0;
};