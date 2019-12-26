// #include "Image.h"

///////////////////////////// Objets d'images

template<typename T>
void ImageBase<T>::resize(size_t width, size_t height)
{
	m_data.resize(width * height);
	m_width = width;
	m_height = height;
}

template<typename T>
void ImageBase<T>::load(const std::string& filename)
{

	std::ifstream in_file(filename, std::ios::binary);
	
	if (!in_file.is_open())
	{
		std::cerr << "Error : failure in load while opening file\n";
		return;
	}

	this->read(in_file);

	in_file.close();
}

template<typename T>
void ImageBase<T>::save(const std::string& filename)
{
	std::ofstream out_file(filename, std::ios::binary);

	if (!out_file.is_open())
	{
		std::cerr << "Error : failure in save while opening file\n";
		return;
	}

	this->write(out_file);

	out_file.close();
}

template<typename U>
std::istream& operator>>(std::istream& is, ImageBase<U>& image)
{
	image.read(is);
	return is;
}

template<typename U>
std::ostream& operator<<(std::ostream& os, const ImageBase<U>& image)
{
	image.write(os);
	return os;
}

template<typename T>
void ImageIdentifiable<T>::read(std::istream& is)
{	
	std::string image_identifier;
	size_t max_intensity;

	is >> image_identifier;

	if (image_identifier != this->identifier())
	{
		std::cerr << "Error : wrong image format (" << image_format_identifier(image_identifier) << " instead of " << image_format_identifier(this->identifier()) << ")\n";
		exit(EXIT_FAILURE);
	}

	ignore_comment(is);
	is >> this->m_width >> this->m_height;
	is >> max_intensity;
	is.ignore();

	this->m_data.resize(this->width() * this->height());

	is.read((char*)this->data(), this->size() * sizeof(T));

	if (!is.good())
		std::cerr << "Error : failure while reading image from stream\n";
}

template<typename T>
void ImageIdentifiable<T>::write(std::ostream& os) const
{
	os << this->identifier() << "\r";
	os << this->width() << " " << this->height() << "\r";
	os << "255\r";

	os.write((char*)this->data(), this->size() * sizeof(T));

	if (!os.good())
		std::cerr << "Error : failure while writing image to stream\n";
}

ImagePPM::ImagePPM(const ImagePGM& red, const ImagePGM& green, const ImagePGM& blue)
{
	if (!(red.same_dimension(green) && red.same_dimension(blue)))
	{
		std::cerr << "Error : Images dimensions are not the sames\n";
		exit(EXIT_FAILURE);
	}

	this->resize(red.width(), red.height());

	for (size_t i = 0 ; i < this->size() ; ++i)
	{
		(*this)[i].r = red[i];
		(*this)[i].g = green[i];
		(*this)[i].b = blue[i];
	}
}

ImagePGM ImagePPM::red() const
{
	ImagePGM red(this->width(), this->height());

	for (size_t i = 0 ; i < this->size() ; ++i)
	{
		red[i] = (*this)[i].r;
	}

	return std::move(red);
}

ImagePGM ImagePPM::green() const
{
	ImagePGM green(this->width(), this->height());
	
	for (size_t i = 0 ; i < this->size() ; ++i)
	{
		green[i] = (*this)[i].g;
	}

	return std::move(green);
}

ImagePGM ImagePPM::blue() const
{
	ImagePGM blue(this->width(), this->height());
	
	for (size_t i = 0 ; i < this->size() ; ++i)
	{
		blue[i] = (*this)[i].b;
	}

	return std::move(blue);
}

///////////////////////////// Fonctions d'images

bool check_image_type(const std::string& filename, const std::string& image_type)
{
	std::ifstream in_file(filename, std::ios::binary);
	
	if (!in_file.is_open()) {
		std::cerr << "Error : failure at load while opening file\n";
		return false;
	}

	std::string file_type;

	if(!(in_file >> file_type))
	{
		return false;
	}

	in_file.close();

	return file_type == image_type;
}

const std::string image_format_identifier(const std::string& identifier)
{
	if (identifier == "P5")
		return "PGM";
	
	else if (identifier == "P6")
		return "PPM";

	return "Unknown";
}

bool is_pgm(const std::string& filename)
{
	return check_image_type(filename, "P5");
}

bool is_ppm(const std::string& filename)
{
	return check_image_type(filename, "P6");
}

void ignore_comment(std::istream& is)
{	
	OCTET c = is.get();

	if (c != '\r' && c != '\n')
		is.seekg(-sizeof(OCTET), is.cur);
  
	while ((c = is.get()) == '#')
	{
		std::cerr << "Ignoring : #";
		while ((c = is.get()) != '\n' && c != '\r')
		{
			std::cerr << (char)c;
		}
		std::cerr << "\n";
	}

	if (c != '\r' && c != '\n')
		is.seekg(-sizeof(OCTET), is.cur);
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

///////////////////////////// Objets de couleurs

template<typename T>
Color<T>::Color() : r(0), g(0), b(0) {}

template<typename T>
Color<T>::Color(OCTET r, OCTET g, OCTET b) : r(r), g(g), b(b) {}

template<typename T>
Color<T> Color<T>::operator+(const Color<T>& c) const
{
	return Color<T>(
		this->r + c.r,
		this->g + c.g,
		this->b + c.b
	);
}

template<typename T>
Color<T>& Color<T>::operator+=(const Color<T>& c)
{
	this->r = this->r + c.r;
	this->g = this->g + c.g;
	this->b = this->b + c.b;
	return *this;
}

template<typename T>
Color<T> Color<T>::operator-(const Color<T>& c) const
{
	return Color<T>(
		this->r - c.r,
		this->g - c.g,
		this->b - c.b
	);
}

template<typename T>
Color<T>& Color<T>::operator-=(const Color<T>& c)
{
	this->r = this->r - c.r;
	this->g = this->g - c.g;
	this->b = this->b - c.b;
	return *this;
}

template<typename T>
Color<T> Color<T>::operator*(const Color<T>& c) const
{
	return Color<T>(
		this->r * c.r,
		this->g * c.g,
		this->b * c.b
	);
}

template<typename T>
Color<T>& Color<T>::operator*=(const Color<T>& c)
{
	this->r = this->r * c.r;
	this->g = this->g * c.g;
	this->b = this->b * c.b;
	return *this;
}

template<typename T>
Color<T> Color<T>::operator/(const Color<T>& c) const
{
	return Color<T>(
		this->r / c.r,
		this->g / c.g,
		this->b / c.b
	);
}

template<typename T>
Color<T>& Color<T>::operator/=(const Color<T>& c)
{
	this->r = this->r / c.r;
	this->g = this->g / c.g;
	this->b = this->b / c.b;
	return *this;
}

template<typename T>
Color<T> Color<T>::operator*(double n) const
{
	return Color<T>(
		this->r * n,
		this->g * n,
		this->b * n
	);
}

template<typename T>
Color<T>& Color<T>::operator*=(double n)
{
	this->r = this->r * n;
	this->g = this->g * n;
	this->b = this->b * n;
	return *this;
}

template<typename T>
Color<T> Color<T>::operator/(double n) const
{
	return Color<T>(
		this->r / n,
		this->g / n,
		this->b / n
	);
}

template<typename T>
Color<T>& Color<T>::operator/=(double n)
{
	this->r = this->r / n;
	this->g = this->g / n;
	this->b = this->b / n;
	return *this;
}

template<typename T>
bool Color<T>::operator<(const Color& c) const
{
	if (this->r < c.r)
		return true;
	else if (this->r > c.r)
		return false;
	else
		if (this->g < c.g)
			return true;
		else if (this->g > c.g)
			return false;
		else
			return this->b < c.b;
}

template<typename T>
template<typename U>
Color<T>::operator Color<U>() const
{
	return Color<U>(static_cast<U>(this->r), static_cast<U>(this->g), static_cast<U>(this->b));
}

template<typename T>
Color<T>::operator OCTET*()
{
	return &r;
}

template<typename T>
Color<T> Color<T>::black(0, 0, 0);

template<typename T>
Color<T> Color<T>::white(255, 255, 255);

template<typename U>
std::istream& operator>>(std::istream& is, Color<U>& c)
{
	is >> c.r >> c.g >> c.b;
	return is;
}

template<typename U>
std::ostream& operator<<(std::ostream& os, const Color<U>& c)
{
	return os << c.r << " " << c.g << " " << c.b;
}