#ifndef IMAGE_BASE_H
#define IMAGE_BASE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <array>
#include <cmath>
#include <cassert>
#include <map>
#include <algorithm>

typedef unsigned char OCTET;

///////////////////////////// Objets d'images

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

template<typename T>
class ImageIdentifiable : public ImageBase<T>
{
	using ImageBase<T>::ImageBase;


	void read(std::istream& is);
	void write(std::ostream& os) const;

public:
	
	virtual const std::string identifier() const = 0;
};

class ImagePGM : public ImageIdentifiable<OCTET>
{
	using ImageIdentifiable::ImageIdentifiable;

public:

	const std::string identifier() const { return "P5"; }
};

template<typename T>
struct Color;

typedef Color<OCTET>	RGB;
typedef Color<float>	RGBf;
typedef Color<double>	RGBd;

class ImagePPM : public ImageIdentifiable<RGB>
{
	using ImageIdentifiable::ImageIdentifiable;

public:

	ImagePPM(const ImagePGM& red, const ImagePGM& green, const ImagePGM& blue);

	ImagePGM red() const;
	ImagePGM green() const;
	ImagePGM blue() const;

	const std::string identifier() const { return "P6"; }
};

///////////////////////////// Fonctions d'images

const std::string image_format_identifier(const std::string& identifier);

bool is_pgm(const std::string& filename);
bool is_ppm(const std::string& filename);

void ignore_comment(std::istream& is);
void ignore_characters(std::istream& is);

///////////////////////////// Objets de couleurs

template<typename T>
struct Color
{
	T r, g, b;

	Color();
	Color(OCTET r, OCTET g, OCTET b);

	// Operators overloads

	Color operator+(const Color& c) const;
	Color& operator+=(const Color& c);

	Color operator-(const Color& c) const;
	Color& operator-=(const Color& c);

	Color operator*(const Color& c) const;
	Color& operator*=(const Color& c);

	Color operator/(const Color& c) const;
	Color& operator/=(const Color& c);

	Color operator*(double n) const;
	Color& operator*=(double n);

	Color operator/(double n) const;
	Color& operator/=(double n);

	bool operator<(const Color& c) const;
	
	// Cast operators
	
	template<typename U>
	operator Color<U>() const;

	// Read/Write functions

	template<typename U>
	friend std::istream& operator>>(std::istream& is, Color<U>& c);
	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, const Color<U>& c);

	static Color black;
	static Color white;

	operator OCTET*();
};

#include "Image.tpp"

#endif