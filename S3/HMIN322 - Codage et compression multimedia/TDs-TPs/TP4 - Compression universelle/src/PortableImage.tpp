#include "PortableImage.hpp"
#include <limits>
#include <cassert>
#include <cmath>


///////////////////////////////////////////////// PortableImageHeader 

template<const char* FORMAT_IDENTIFIER>
const char* PortableImageHeader<FORMAT_IDENTIFIER>::format_id()
{
	return FORMAT_IDENTIFIER;
}

template<const char* FORMAT_IDENTIFIER>
void PortableImageHeader<FORMAT_IDENTIFIER>::load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	
	assert(file.is_open());

	this->read(file);

	file.close();
}

template<const char* FORMAT_IDENTIFIER>
void PortableImageHeader<FORMAT_IDENTIFIER>::save(const std::string& filename)
{
	std::ofstream file(filename, std::ios::binary);
	
	assert(file.is_open());

	this->write(file);

	file.close();
}

template<const char* FORMAT_IDENTIFIER>
void PortableImageHeader<FORMAT_IDENTIFIER>::read(std::istream& is)
{
	std::string format_id;
	
	is >> format_id;
	
	assert(format_id == PortableImageHeader<FORMAT_IDENTIFIER>::format_id() && "incompatible image header format");

	ignore_comments(is);

	is >> width >> height >> max_intensity;

	is.ignore();
}

template<const char* FORMAT_IDENTIFIER>
void PortableImageHeader<FORMAT_IDENTIFIER>::write(std::ostream& os) const
{
	fprintf(stderr, "possible source d'erreur ici !\n");
	os << PortableImageHeader<FORMAT_IDENTIFIER>::format_id() << '\r';
	os << width << ' ' << height << '\r';
	os << max_intensity << '\r';
}


///////////////////////////////////////////////// Generic Portable Image


// Constructors

template<typename T, typename IMAGE_HEADER>
PortableImage<T, IMAGE_HEADER>::PortableImage(const std::string& filename)
{
	this->load(filename);
}

// Image type

template<typename T, typename IMAGE_HEADER>
size_t PortableImage<T, IMAGE_HEADER>::dimension() const
{
	return type_dimension<T>();
}

template<typename T, typename IMAGE_HEADER>
typename PortableImage<T, IMAGE_HEADER>::header_type PortableImage<T, IMAGE_HEADER>::header() const
{
	return {this->width(), this->height(), type_max_intensity<T>()};
}

// Read/Write functions

template<typename T, typename IMAGE_HEADER>
void PortableImage<T, IMAGE_HEADER>::load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	
	assert(file.is_open());

	this->read(file);

	file.close();
}

template<typename T, typename IMAGE_HEADER>
void PortableImage<T, IMAGE_HEADER>::save(const std::string& filename)
{
	std::ofstream file(filename, std::ios::binary);

	assert(file.is_open());

	this->write(file);

	file.close();
}

template<typename T, typename IMAGE_HEADER>
std::istream& operator>>(std::istream& is, PortableImage<T, IMAGE_HEADER>& image)
{
	image.read(is);
	return is;
}

template<typename T, typename IMAGE_HEADER>
std::ostream& operator<<(std::ostream& os, const PortableImage<T, IMAGE_HEADER>& image)
{
	image.write(os);
	return os;
}

template<typename T, typename IMAGE_HEADER>
void PortableImage<T, IMAGE_HEADER>::read(std::istream& is)
{	
	IMAGE_HEADER header; header.read(is);

	assert(header.max_intensity < std::numeric_limits<T>::max() && "max image intensity too big for image type");

	this->resize(header.height, header.width);

	is.read((char*)this->data(), this->size() * sizeof(T));

	assert(is.good() && "Error : failure while reading image from stream");
}

template<typename T, typename IMAGE_HEADER>
void PortableImage<T, IMAGE_HEADER>::write(std::ostream& os) const
{
	this->header().write(os);

	os.write((char*)this->data(), this->size() * sizeof(T));

	assert(os.good() && "Error : failure while writing image to stream");
}

///////////////////////////////////////////////// COLOR

// Constructors

template<typename T>
GenericRGB<T>::GenericRGB() : r(0), g(0), b(0) {}

template<typename T>
GenericRGB<T>::GenericRGB(T r, T g, T b) : r(r), g(g), b(b) {}

// Const operators

template<typename T>
GenericRGB<T> GenericRGB<T>::operator+(const GenericRGB<T>& c) const
{
	return GenericRGB<T>(
		this->r + c.r,
		this->g + c.g,
		this->b + c.b
	);
}

template<typename T>
GenericRGB<T> GenericRGB<T>::operator-(const GenericRGB<T>& c) const
{
	return GenericRGB<T>(
		this->r - c.r,
		this->g - c.g,
		this->b - c.b
	);
}

template<typename T>
GenericRGB<T> GenericRGB<T>::operator*(const GenericRGB<T>& c) const
{
	return GenericRGB<T>(
		this->r * c.r,
		this->g * c.g,
		this->b * c.b
	);
}

template<typename T>
GenericRGB<T> GenericRGB<T>::operator/(const GenericRGB<T>& c) const
{
	return GenericRGB<T>(
		this->r / c.r,
		this->g / c.g,
		this->b / c.b
	);
}

template<typename T>
template<typename U>
GenericRGB<T> GenericRGB<T>::operator*(const U& value) const
{
	return GenericRGB<T>(
		this->r * value,
		this->g * value,
		this->b * value
	);
}



template<typename T>
template<typename U>
GenericRGB<T> GenericRGB<T>::operator/(const U& value) const
{
	return GenericRGB<T>(
		this->r / value,
		this->g / value,
		this->b / value
	);
}

// Assignement operators

template<typename T>
GenericRGB<T>& GenericRGB<T>::operator+=(const GenericRGB<T>& c)
{
	this->r = this->r + c.r;
	this->g = this->g + c.g;
	this->b = this->b + c.b;
	return *this;
}

template<typename T>
GenericRGB<T>& GenericRGB<T>::operator-=(const GenericRGB<T>& c)
{
	this->r = this->r - c.r;
	this->g = this->g - c.g;
	this->b = this->b - c.b;
	return *this;
}



template<typename T>
GenericRGB<T>& GenericRGB<T>::operator*=(const GenericRGB<T>& c)
{
	this->r = this->r * c.r;
	this->g = this->g * c.g;
	this->b = this->b * c.b;
	return *this;
}

template<typename T>
GenericRGB<T>& GenericRGB<T>::operator/=(const GenericRGB<T>& c)
{
	this->r = this->r / c.r;
	this->g = this->g / c.g;
	this->b = this->b / c.b;
	return *this;
}

template<typename T>
template<typename U>
GenericRGB<T>& GenericRGB<T>::operator*=(const U& value)
{
	this->r = this->r * value;
	this->g = this->g * value;
	this->b = this->b * value;
	return *this;
}

template<typename T>
template<typename U>
GenericRGB<T>& GenericRGB<T>::operator/=(const U& value)
{
	this->r = this->r / value;
	this->g = this->g / value;
	this->b = this->b / value;
	return *this;
}

// Bool operators

template<typename T>
bool GenericRGB<T>::operator<(const GenericRGB<T>& c) const
{
	if (this->r < c.r)
	{
		return true;
	}
	else if (this->r == c.r)
	{
		if (this->g < c.g)
		{
			return true;
		}
		else if (this->g == c.g)
		{
			return this->b < c.b;
		}
	}
	return false;
}

// Cast operators

template<typename T>
template<typename U>
GenericRGB<T>::operator GenericRGB<U>() const
{
	return GenericRGB<U>(static_cast<U>(this->r), static_cast<U>(this->g), static_cast<U>(this->b));
}

template<typename T>
GenericRGB<T>::operator T*()
{
	return &r;
}

// Static members

template<typename T>
GenericRGB<T> GenericRGB<T>::black(0, 0, 0);

template<typename T>
GenericRGB<T> GenericRGB<T>::white(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());

template<typename T>
std::istream& operator>>(std::istream& is, GenericRGB<T>& c)
{
	is >> c.r >> c.g >> c.b;
	return is;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const GenericRGB<T>& c)
{
	return os << c.r << " " << c.g << " " << c.b;
}


///////////////////////////////////////////////// PPM 


template<typename T>
GenericPPM<T>::GenericPPM(const GenericPGM< typename T::value_type >& red, const GenericPGM< typename T::value_type >& green, const GenericPGM< typename T::value_type >& blue)
{
	assert(red.same_dimension(green) && red.same_dimension(blue));

	this->resize(red.width(), red.height());

	for (size_t i = 0 ; i < this->size() ; ++i)
	{
		(*this)[i].r = red[i];
		(*this)[i].g = green[i];
		(*this)[i].b = blue[i];
	}
}

template<typename T>
GenericPGM< typename T::value_type > GenericPPM<T>::red() const
{
	GenericPGM< typename T::value_type > red(this->width(), this->height());

	for (size_t i = 0 ; i < this->size() ; ++i)
	{
		red[i] = (*this)[i].r;
	}

	return red;
}

template<typename T>
GenericPGM< typename T::value_type > GenericPPM<T>::green() const
{
	GenericPGM< typename T::value_type > green(this->width(), this->height());
	
	for (size_t i = 0 ; i < this->size() ; ++i)
	{
		green[i] = (*this)[i].g;
	}

	return green;
}

template<typename T>
GenericPGM< typename T::value_type > GenericPPM<T>::blue() const
{
	GenericPGM< typename T::value_type > blue(this->width(), this->height());
	
	for (size_t i = 0 ; i < this->size() ; ++i)
	{
		blue[i] = (*this)[i].b;
	}

	return blue;
}


///////////////////////////////////////////////// Helper functions


// Pour les types arithmetiques (char, int, double, etc...)
template<typename T, typename std::enable_if_t<std::is_arithmetic<T>::value, T>*>
constexpr size_t type_dimension(){return 1;}

// Pour les objets integrant les typedef standard (using type = ...; et using value_type = ...) la taille de l'objet doit être connu à la compilation
template<typename T, typename std::enable_if_t<std::is_class<T>::value, T>*> 
constexpr size_t type_dimension(){return sizeof(typename T::type) / sizeof(typename T::value_type);}

// Pour les types arithmetiques (char, int, double, etc...)
template<typename T, typename std::enable_if_t<std::is_arithmetic<T>::value, T>*>
constexpr size_t type_max_intensity(){return std::numeric_limits<T>::max();}

// Pour les objets integrant les typedef standard (using type = ...; et using value_type = ...) la taille de l'objet doit être connu à la compilation
template<typename T, typename std::enable_if_t<std::is_class<T>::value, T>*> 
constexpr size_t type_max_intensity(){return std::numeric_limits<typename T::value_type>::max();}


void ignore_comments(std::istream& is)
{	
	unsigned char c = is.get();

	if (c != '\r' && c != '\n')
		is.seekg(-sizeof(unsigned char), is.cur);
  
	while ((c = is.get()) == '#')
	{
		fprintf(stderr, "Ignoring : #");
		while ((c = is.get()) != '\n' && c != '\r')
		{
			fprintf(stderr, "%c", (char)c);
		}
		fprintf(stderr, "\n");
	}

	if (c != '\r' && c != '\n')
		is.seekg(-sizeof(unsigned char), is.cur);
}

void ignore_characters(std::istream& is)
{
	char c;

	while (is >> c && !is.fail())
	{
		is.clear();
	}
	
	is.seekg(-sizeof(char), is.cur);
}

bool is_pgm_header(const PortableImageHeader& header)
{
	return header.format_id == PGM_FORMAT_ID;
}

bool is_ppm_header(const PortableImageHeader& header)
{
	return header.format_id == PPM_FORMAT_ID;
}
