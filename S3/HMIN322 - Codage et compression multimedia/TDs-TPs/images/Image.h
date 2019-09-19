#ifndef IMAGE_BASE_H
#define IMAGE_BASE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <cmath>

//#include "image_pgm_ppm.h"

typedef unsigned char OCTET;

class ImageBase {

public:

	ImageBase();
	ImageBase(size_t width, size_t height);
	ImageBase(const std::string& filename);

	size_t height() const;
	size_t width() const;
	size_t size() const;

	bool valid() const;
	bool on_bounds(size_t row, size_t column) const;
	bool same_resolution(const ImageBase& image) const;

	void load(const std::string& filename);
	void save(const std::string& filename);

	friend std::istream& operator>>(std::istream& is, ImageBase& image);
	friend std::ostream& operator<<(std::ostream& os, const ImageBase& image);

protected:

	size_t m_height;
	size_t m_width;
	bool m_valid;

	// charge une image en fonction de sont type (.ppm / .pgm)
	virtual void read(std::istream& is) = 0;
	virtual void write(std::ostream& os) const = 0;
};

////////////////////////////////// ImagePGM ////////////////////////////////////////

template <typename T>
class Image : public ImageBase {

public:

	typedef T* iterator;
    typedef const T* const_iterator;

	Image();
	Image(size_t width, size_t height);
	Image(const std::string& filename);

	void resize(size_t width, size_t height);

	// Access data in array style pgm(n)
	T& operator[](size_t n);
	const T& operator[](size_t n) const;

	// Access data in matrix style pgm(i, j)
	T& operator()(size_t row, size_t column);
	const T& operator()(size_t row, size_t column) const;

	// Accessor of internal data
	T* data();
	const T* data() const;

	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;
	
protected:

	std::vector<T> m_data;

	void read(std::istream& is);
	void write(std::ostream& os) const;
};

////////////////////////////////// ImagePPM ////////////////////////////////////////

struct Color
{
	OCTET r, g, b;

	Color();
	Color(const OCTET rgb[3]);
	Color(OCTET r, OCTET g = 0, OCTET b = 0);

	Color operator+(const Color& c) const;
	Color& operator+=(const Color& c);

	Color operator-(const Color& c) const;
	Color& operator-=(const Color& c);

	static Color black;
	static Color white;

	operator OCTET*();
};

std::istream& operator>>(std::istream& is, Color& c);
std::ostream& operator<<(std::ostream& os, const Color& c);

class ImagePPM : public ImageBase {

public:	
	typedef OCTET* iterator;
    typedef const OCTET* const_iterator;

	ImagePPM();
	ImagePPM(size_t width, size_t height);
	ImagePPM(const std::string& filename);
	ImagePPM(const ImagePGM& red, const ImagePGM& green, const ImagePGM& blue);
	
	Color& operator[](size_t n);
	const Color& operator[](size_t n) const;

	Color& operator()(size_t row, size_t column);
	const Color& operator()(size_t row, size_t column) const;
/*
	OCTET& red(size_t n);
	OCTET& red(size_t row, size_t column);
	const OCTET& red(size_t n) const;
	const OCTET& red(size_t row, size_t column) const;

	OCTET& green(size_t n);
	OCTET& green(size_t row, size_t column);
	const OCTET& green(size_t n) const;
	const OCTET& green(size_t row, size_t column) const;

	OCTET& blue(size_t n);
	OCTET& blue(size_t row, size_t column);
	const OCTET& blue(size_t n) const;
	const OCTET& blue(size_t row, size_t column) const;

	// Getters/Setters pour checker que les resolutions des parties R/G/B de l'image sont les mêmes.

	const ImagePGM& red() const;
	const ImagePGM& green() const;
	const ImagePGM& blue() const;

	void red(const ImagePGM& red);
	void green(const ImagePGM& green);
	void blue(const ImagePGM& blue);

	void map(void (*func)(OCTET&));
	void map(void (*func)(const OCTET&)) const;
*/
	// Accessor of internal data
	Color* data();
	const Color* data() const;

	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;

protected:

/*
	ImagePGM m_red;
	ImagePGM m_green;
	ImagePGM m_blue;
*/
	std::vector<Color> m_data;

	void read(std::istream& is);
	void write(std::ostream& os) const;
};

void seuillage(ImagePGM& image, const std::vector<int>& seuils);

std::vector<int> histo_grey(const ImagePGM& image);
std::vector<std::array<int, 3> > histo_color(const ImagePPM& image);

void print_histo_grey(std::vector<int> histo_grey);
void print_histo_color(std::vector<std::array<int, 3> > histo_color);

bool has_suffix(const std::string &str, const std::string &suffix);

void min_max_grey(const std::vector<int>& histo, int& min, int& max);

void expansion(ImagePGM& image, int min, int max);

std::string remove_extension(const std::string& filename);

void seuillage_extrema(ImagePGM& image, int min, int max);

std::vector<double> ddp_grey(const std::vector<int>& histo, long nb_pixels);
void print_ddp_grey(const std::vector<double>& histo_grey);

std::vector<double> repartition_grey(const std::vector<double>& ddp);
std::vector<double> repartition_inverse_grey(const std::vector<double>& repartition);

void egalisation(ImagePGM& image, std::vector<double> repartition);
void specification(ImagePGM& image, std::vector<double> repartition);

bool is_pgm(const std::string& filename);
bool is_ppm(const std::string& filename);

#endif