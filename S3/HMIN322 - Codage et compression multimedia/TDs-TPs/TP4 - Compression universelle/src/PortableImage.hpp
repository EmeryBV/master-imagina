#ifndef PORTABLE_IMAGE_HPP
#define PORTABLE_IMAGE_HPP

#include <fstream>
#include <type_traits>

#include "Matrix.hpp"

// le mot clef "final" permet d'optimiser les methodes virtuelles
// variant permet d'implementer efficacement un type dynamique avec tu pattern matchting

// pixel_depth _max_intensity
// image_format_signature

// Lecture du fichier avec header
// format_id
// width
// height
// max_intensity
// 


// lit byte par byte et regarde si l'identifiant est reconnue dans la base de donnée d'identifiants
// parse_image_id(filename) -> renvoie l'identifiant ou échoue si l'identifiant n'est pas dans la base de donnée

// std:variant<PGMuchar, PPMuchar> load_image(istream& is)
// {
//  	PortableImageHeader header(is)
//  	
//  	if (header.format_id() == PGM_FORMAT_ID)
//  	{
//  		if (header.max_intensity() == 255)
//  		{
//  			return PGMuchar(header, is);
//  		}
//  	}
//  	
//  	if (header.format_id() == PPM_FORMAT_ID)
//  	{
//  		if (header.max_intensity() == 255)
//  		{
//  			return PPMuchar(header, is);
//  		}
//  	}
// }
// 
// 
// 
// Probleme avec ennemi collision
// Ennemi attaque scripts
// Script qui gere l'ouverture des portes
// Truc qui montre l'orientation du perso
// Ouvrir des issues

///////////////////////////////////////////////// PortableImageHeader 


struct PortableImageHeader
{
	std::string format_id;
	size_t width;
	size_t height;
	size_t max_intensity;

	PortableImageHeader(size_t width, size_t height, size_t max_intensity);

	// Read/Write functions

	void load(const std::string& filename);
	void save(const std::string& filename);

	void read(std::istream& is);
	void write(std::ostream& os) const;

	static const char* format_id();
};

static const char PGM_FORMAT_ID[] = "P5";
static const char PPM_FORMAT_ID[] = "P6";
static const char PNG_FORMAT_ID[] = "\211PNG\r\n\032\n";

using PGMHeader = PortableImageHeader<PGM_FORMAT_ID>;
using PPMHeader = PortableImageHeader<PPM_FORMAT_ID>;


///////////////////////////////////////////////// Generic Portable Image


template<typename T, typename PORTABLE_IMAGE_HEADER>
struct PortableImage : public Matrix<T>
{
	// Constructors
	
	using Matrix<T>::Matrix;
	using value_type  = T;
	using header_type = PORTABLE_IMAGE_HEADER;

	PortableImage(const std::string& filename);

	// Image type

	size_t dimension() const; 	// Image dimension : 1 -> (Gray), 3 -> (RGB, YCrCb, ...), etc...
	header_type header() const;

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


template<typename T>
using GenericPGM = PortableImage<T, PGM_FORMAT_ID>;


///////////////////////////////////////////////// COLOR


template<typename T>
struct GenericRGB
{
	using value_type = T; 

	T r, g, b;

	// Constructors

	GenericRGB();
	GenericRGB(T r, T g, T b);

	// Const operators

	GenericRGB operator+(const GenericRGB& c) const;
	GenericRGB operator-(const GenericRGB& c) const;
	GenericRGB operator*(const GenericRGB& c) const;
	GenericRGB operator/(const GenericRGB& c) const;

	template<typename U>
	GenericRGB operator*(const U& value) const;
	template<typename U>
	GenericRGB operator/(const U& value) const;

	// Assignement operators
	
	GenericRGB& operator+=(const GenericRGB& c);
	GenericRGB& operator-=(const GenericRGB& c);
	GenericRGB& operator*=(const GenericRGB& c);
	GenericRGB& operator/=(const GenericRGB& c);

	template<typename U>
	GenericRGB& operator*=(const U& value);
	template<typename U>
	GenericRGB& operator/=(const U& value);

	// Bool operators

	bool operator<(const GenericRGB& c) const;
	
	// Cast operators
	
	template<typename U>
	explicit operator GenericRGB<U>() const;
	operator T*();

	// Static members

	static GenericRGB black;
	static GenericRGB white;
};

template<typename T>
std::istream& operator>>(std::istream& is, GenericRGB<T>& c);
template<typename T>
std::ostream& operator<<(std::ostream& os, const GenericRGB<T>& c);


///////////////////////////////////////////////// PPM 



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

// #include "PortableImage.tpp"

using PGM  = GenericPGM<unsigned char>;
using PGMi = GenericPGM<unsigned int>;
using PGMf = GenericPGM<float>;
using PGMd = GenericPGM<double>;

using RGB  = GenericRGB<unsigned char>;
using RGBi = GenericRGB<unsigned int>;
using RGBf = GenericRGB<float>;
using RGBd = GenericRGB<double>;

using PPM  = GenericPPM<RGB>;
using PPMi = GenericPPM<RGBi>;
using PPMf = GenericPPM<RGBf>;
using PPMd = GenericPPM<RGBd>;

#endif