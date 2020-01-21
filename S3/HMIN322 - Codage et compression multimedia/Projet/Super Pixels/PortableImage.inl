#ifndef PORTABLE_IMAGE_INL
#define PORTABLE_IMAGE_INL

#include "PortableImage.hpp"
#include <limits>
#include <cassert>
#include <cmath>


///////////////////////////////////////////////// PortableImageHeader 


void PortableImageHeader::load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	
	assert(file.is_open());

	this->read(file);

	file.close();
}

void PortableImageHeader::save(const std::string& filename)
{
	std::ofstream file(filename, std::ios::binary);
	
	assert(file.is_open());

	this->write(file);

	file.close();
}

void PortableImageHeader::read(std::istream& is)
{
	is >> format_id;
	
	ignore_comments(is);

	is >> width >> height >> max_intensity;

	is.ignore();
}

void PortableImageHeader::write(std::ostream& os) const
{
	os << format_id << '\r';
	os << width << ' ' << height << '\r';
	os << max_intensity << '\r';
}


///////////////////////////////////////////////// Generic Portable Image


// Constructors

template<typename T, const char* FORMAT_IDENTIFIER>
PortableImage<T, FORMAT_IDENTIFIER>::PortableImage(const std::string& filename)
{
	this->load(filename);
}

// Image type

template<typename T, const char* FORMAT_IDENTIFIER>
size_t PortableImage<T, FORMAT_IDENTIFIER>::dimension() const
{
	return type_dimension<T>();
}

template<typename T, const char* FORMAT_IDENTIFIER>
const char* PortableImage<T, FORMAT_IDENTIFIER>::format_id() const
{
	return FORMAT_IDENTIFIER;
}

template<typename T, const char* FORMAT_IDENTIFIER>
PortableImageHeader PortableImage<T, FORMAT_IDENTIFIER>::header() const
{
	return {format_id(), this->width(), this->height(), type_max_intensity<T>()};
}

// Read/Write functions

template<typename T, const char* FORMAT_IDENTIFIER>
void PortableImage<T, FORMAT_IDENTIFIER>::load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	
	assert(file.is_open());

	this->read(file);

	file.close();
}

template<typename T, const char* FORMAT_IDENTIFIER>
void PortableImage<T, FORMAT_IDENTIFIER>::save(const std::string& filename)
{
	std::ofstream file(filename, std::ios::binary);

	assert(file.is_open());

	this->write(file);

	file.close();
}

template<typename T, const char* FORMAT_IDENTIFIER>
std::istream& operator>>(std::istream& is, PortableImage<T, FORMAT_IDENTIFIER>& image)
{
	image.read(is);
	return is;
}

template<typename T, const char* FORMAT_IDENTIFIER>
std::ostream& operator<<(std::ostream& os, const PortableImage<T, FORMAT_IDENTIFIER>& image)
{
	image.write(os);
	return os;
}

template<typename T, const char* FORMAT_IDENTIFIER>
void PortableImage<T, FORMAT_IDENTIFIER>::read(std::istream& is)
{	
	PortableImageHeader header; header.read(is);

	assert(header.format_id == this->format_id() && "Error : incompatible image format");
	assert(header.max_intensity < pow(2, 8 * sizeof(T)) && "Error : max image intensity too big for image type");

	this->resize(header.height, header.width);

	is.read((char*)this->data(), this->size() * sizeof(T));

	assert(is.good() && "Error : failure while reading image from stream");
}

template<typename T, const char* FORMAT_IDENTIFIER>
void PortableImage<T, FORMAT_IDENTIFIER>::write(std::ostream& os) const
{
	this->header().write(os);

	os.write((char*)this->data(), this->size() * sizeof(T));

	assert(os.good() && "Error : failure while writing image to stream");
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

#endif