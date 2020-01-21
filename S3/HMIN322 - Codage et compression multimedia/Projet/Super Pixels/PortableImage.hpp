#ifndef PORTABLE_IMAGE_HPP
#define PORTABLE_IMAGE_HPP

#include <valarray>
#include <fstream>
#include <type_traits>

#include "Matrix.hpp"
#include "RGB.hpp"


///////////////////////////////////////////////// PortableImageHeader 


struct PortableImageHeader
{
	std::string format_id;
	size_t width;
	size_t height;
	size_t max_intensity;

	// Read/Write functions

	void load(const std::string& filename);
	void save(const std::string& filename);

	void read(std::istream& is);
	void write(std::ostream& os) const;
};


///////////////////////////////////////////////// Generic Portable Image


template<typename T, const char* FORMAT_IDENTIFIER>
struct PortableImage : public Matrix<T>
{
	// Constructors

	using Matrix<T>::Matrix;

	PortableImage(const std::string& filename);

	// Image type

	size_t dimension() const; 	// Image dimension : 1 -> (Gray), 3 -> (RGB, YCrCb, ...), etc...
	
	const char* format_id() const; // Image format_id : P5 -> PGM, P6 -> PPM, etc...

	PortableImageHeader header() const;

	// Read/Write functions (charge les images ppm, pgm et pbm)
	
	void load(const std::string& filename);
	void save(const std::string& filename);

	void read(std::istream& is);
	void write(std::ostream& os) const;
};

template<typename T, const char* FORMAT_IDENTIFIER>
std::istream& operator>>(std::istream& is, PortableImage<T, FORMAT_IDENTIFIER>& image);

template<typename T, const char* FORMAT_IDENTIFIER>
std::ostream& operator<<(std::ostream& os, const PortableImage<T, FORMAT_IDENTIFIER>& image);


///////////////////////////////////////////////// PGM 


static const char PGM_FORMAT_ID[] = "P5";

template<typename T>
using GenericPGM = PortableImage<T, PGM_FORMAT_ID>;


///////////////////////////////////////////////// PPM 


static const char PPM_FORMAT_ID[] = "P6";

template<typename T>
struct GenericPPM : public PortableImage<T, PPM_FORMAT_ID>
{
	// Constructors

	using PortableImage<T, PPM_FORMAT_ID>::PortableImage;

	GenericPPM(const GenericPGM< typename T::value_type >& red, const GenericPGM< typename T::value_type >& green, const GenericPGM< typename T::value_type >& blue);
	
	// Accessors

	GenericPGM< typename T::value_type > red() const;
	GenericPGM< typename T::value_type > green() const;
	GenericPGM< typename T::value_type > blue() const;
};


///////////////////////////////////////////////// Helper functions


// Pour les types arithmetiques (char, int, double, etc...)
template<typename T, typename std::enable_if_t<std::is_arithmetic<T>::value, T>* = nullptr>
constexpr size_t type_dimension();

// Pour les objets integrant les typedef standard (using type = ...; et using value_type = ...) la taille de l'objet doit être connu à la compilation
template<typename T, typename std::enable_if_t<std::is_class<T>::value, T>* = nullptr> 
constexpr size_t type_dimension();

// Pour les types arithmetiques (char, int, double, etc...)
template<typename T, typename std::enable_if_t<std::is_arithmetic<T>::value, T>* = nullptr>
constexpr size_t type_max_intensity();

// Pour les objets integrant les typedef standard (using type = ...; et using value_type = ...) la taille de l'objet doit être connu à la compilation
template<typename T, typename std::enable_if_t<std::is_class<T>::value, T>* = nullptr> 
constexpr size_t type_max_intensity();


void ignore_comments(std::istream& is);
void ignore_characters(std::istream& is);

bool is_pgm_header(const PortableImageHeader& header);
bool is_ppm_header(const PortableImageHeader& header);

#include "PortableImage.inl"

using PGM  = GenericPGM<unsigned char>;
using PGMi = GenericPGM<unsigned int>;
using PGMf = GenericPGM<float>;
using PGMd = GenericPGM<double>;

using PPM  = GenericPPM<RGB>;
using PPMi = GenericPPM<RGBi>;
using PPMf = GenericPPM<RGBf>;
using PPMd = GenericPPM<RGBd>;

#endif