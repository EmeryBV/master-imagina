#include "Image.h"

////////////////////////////////// ImageBase ////////////////////////////////////////

ImageBase::ImageBase() : m_height(0), m_width(0), m_valid(true) {}

ImageBase::ImageBase(size_t width, size_t height) : m_height(height), m_width(width), m_valid(true) {}

ImageBase::ImageBase(const std::string& filename) : m_height(0), m_width(0), m_valid(true) {
	this->load(filename);
}

size_t ImageBase::height() const {
	return m_height;
}

size_t ImageBase::width() const {
	return m_width;
}

size_t ImageBase::size() const {
	return this->width() * this->height();
}

bool ImageBase::valid() const {
	return m_valid;
}

bool ImageBase::on_bounds(size_t row, size_t column) const {
	return row < this->height() && column < this->width();
}

bool ImageBase::same_resolution(const ImageBase& image) const {
	return this->height() == image.height() && this->width() == image.width();
}

void ImageBase::load(const std::string& filename) {

	std::ifstream in_file(filename, std::ios::binary);
	
	if (!in_file.is_open()) {
		std::cerr << "Error : failure at load while opening file\n";
		return;
	}

	this->read(in_file);
	in_file.close();
}

void ImageBase::save(const std::string& filename) {

	std::ofstream out_file(filename, std::ios::binary);

	if (!out_file.is_open()) {
		std::cerr << "Error : failure at save while opening file\n";
		return;
	}

	this->write(out_file);
	out_file.close();
}

std::istream& operator>>(std::istream& is, ImageBase& image) {
	image.read(is);
	return is;
}

std::ostream& operator<<(std::ostream& os, const ImageBase& image) {
	image.write(os);
	return os;
}

////////////////////////////////// ImagePGM ////////////////////////////////////////

ImagePGM::ImagePGM() : ImageBase(), m_data() {}

ImagePGM::ImagePGM(size_t width, size_t height) : ImageBase(width, height), m_data(width * height) {}

ImagePGM::ImagePGM(const std::string& filename) : ImageBase(), m_data() {
	this->load(filename);
}

void ImagePGM::resize(size_t width, size_t height) {
	m_data.resize(width * height);
	m_width = width;
	m_height = height;
}

OCTET& ImagePGM::operator[](size_t n) {
	return m_data[n];
}

const OCTET& ImagePGM::operator[](size_t n) const {
	return m_data[n];
}

OCTET& ImagePGM::operator()(size_t row, size_t column) {
	return this->m_data[row * this->width() + column]; 
}

const OCTET& ImagePGM::operator()(size_t row, size_t column) const {
	return this->m_data[row * this->width() + column];
}

OCTET* ImagePGM::data() {
	return m_data.data(); 
}

const OCTET* ImagePGM::data() const {
	return m_data.data();
}

ImagePGM::iterator ImagePGM::begin() {
	return this->data();
}

ImagePGM::const_iterator ImagePGM::begin() const {
	return this->begin();
}

ImagePGM::iterator ImagePGM::end() {
	return this->data() + this->size();
}

ImagePGM::const_iterator ImagePGM::end() const {
	return this->end();
}

void ignore_comment(std::istream& is) {
	
	OCTET c = is.get();

	if (c != '\r' && c != '\n')
		is.seekg(-sizeof(OCTET), is.cur);
  
	while ((c = is.get()) == '#') {
		std::cerr << "Ignoring : #";
		while ((c = is.get()) != '\n' && c != '\r') {
			std::cerr << (char)c;
		}
		std::cerr << "\n";
	}

	if (c != '\r' && c != '\n')
		is.seekg(-sizeof(OCTET), is.cur);
}

void ignore_characters(std::istream& is) {
	char c;
	while (is >> c && !is.fail()) {
		is.clear();
	}
	is.seekg(-sizeof(char), is.cur);
}

void ImagePGM::read(std::istream& is) {
	
	std::string fmt;
	int max_grey_val;

	is >> fmt; //P5
	ignore_comment(is);
	is >> this->m_width >> this->m_height;
	is >> max_grey_val;
	is.ignore();

	this->m_data.resize(m_height * m_width);

	is.read((char*)this->data(), this->size());

	m_valid = is.good();
}

void ImagePGM::write(std::ostream& os) const {

	std::string fmt;

	os << "P5\r";
	os << this->width() << " " << this->height() << "\r";
	os << "255\r";

	os.write((char*)this->data(), this->size());

	if (!os.good())
		std::cerr << "Warning : failure at save while writing in file\n";
}

void ImagePGM::map(void (*func)(OCTET&)) {
	for (OCTET& o : (*this))
		func(o);
}

void ImagePGM::map(void (*func)(const OCTET&)) const {
	for (const OCTET& o : (*this))
		func(o);
}

ImagePGM ImagePGM::filter(std::array<double, 9> f) {

	
	ImagePGM out(this->width(), this->height());

	long pixel_value = 0;

	for (size_t i = 1; i < this->height() - 1; ++i) {

		for (size_t j = 1; j < this->width() - 1; ++j) {

			pixel_value += (*this)(i-1, j-1);
			pixel_value += (*this)(i-1, j);
			pixel_value += (*this)(i-1, j+1);

			pixel_value += (*this)(i, j+1);
			pixel_value += (*this)(i, j);
			pixel_value += (*this)(i, j-1);

			pixel_value += (*this)(i+1, j-1);
			pixel_value += (*this)(i+1, j);
			pixel_value += (*this)(i+1, j+1);
		}
	}

	return std::move(out);
}


////////////////////////////////// ImagePPM ////////////////////////////////////////

ImagePPM::ImagePPM() : ImageBase(), m_red(), m_green(), m_blue() {}

ImagePPM::ImagePPM(size_t width, size_t height) : ImageBase(width, height), m_red(width, height), m_green(width, height), m_blue(width, height) {}

ImagePPM::ImagePPM(const std::string& filename) : ImageBase() {
	this->load(filename);
}

ImagePPM::ImagePPM(const ImagePGM& red, const ImagePGM& green, const ImagePGM& blue) {
	
	if (red.same_resolution(green) && red.same_resolution(blue)) {
		this->m_red = red;
		this->m_green = green;
		this->m_blue = blue;
		this->m_width = red.width();
		this->m_height = red.height();
	}
}

OCTET& ImagePPM::red(size_t row, size_t column) {
	return this->m_red(row, column);
}

const OCTET& ImagePPM::red(size_t row, size_t column) const {
	return this->m_red(row, column);
}

OCTET& ImagePPM::green(size_t row, size_t column) {
	return this->m_green(row, column);
}

const OCTET& ImagePPM::green(size_t row, size_t column) const {
	return this->m_green(row, column);
}

OCTET& ImagePPM::blue(size_t row, size_t column) {
	return this->m_blue(row, column);
}

const OCTET& ImagePPM::blue(size_t row, size_t column) const {
	return this->m_blue(row, column);
}

const ImagePGM& ImagePPM::red() const {
	return this->m_red;
}

const ImagePGM& ImagePPM::green() const {
	return this->m_green;
}

const ImagePGM& ImagePPM::blue() const {
	return this->m_blue;
}

void ImagePPM::red(const ImagePGM& red) {
	if (this->same_resolution(red))
		this->m_red = red;
}

void ImagePPM::green(const ImagePGM& green) {
	if (this->same_resolution(green))
		this->m_green = green;
}

void ImagePPM::blue(const ImagePGM& blue) {
	if (this->same_resolution(blue))
		this->m_blue = blue;
}

void ImagePPM::read(std::istream& is) {

	std::string fmt;
	int max_grey_val;

	is >> fmt; //P6
	ignore_comment(is);
	is >> this->m_width >> this->m_height;
	is >> max_grey_val;
	is.ignore(); // ignore le charactère qui separe le header des données decalage des couleurs sinon

	std::vector<OCTET> data(this->width() * this->height() * 3);

	is.read((char*)data.data(), data.size());

	this->m_red.resize(this->width(), this->height());
	this->m_green.resize(this->width(), this->height());
	this->m_blue.resize(this->width(), this->height());

	for (size_t i = 0 ; i < this->size(); ++i) {
		this->m_red[i] = data[(i*3)];
		this->m_green[i] = data[(i*3)+1];
		this->m_blue[i] = data[(i*3)+2];
	}

	m_valid = is.good();
}

void ImagePPM::write(std::ostream& os) const {

	std::string fmt;

	os << "P6\r";
	os << this->width() << " " << this->height() << "\r";
	os << "255\r";

	std::vector<OCTET> data(this->width() * this->height() * 3);

	for (size_t i = 0 ; i < this->size() ; ++i) {
		data[(i*3)] = this->m_red[i];
		data[(i*3)+1] = this->m_green[i];
		data[(i*3)+2] = this->m_blue[i];
	}

	os.write((char*)data.data(), data.size());

	if (!os.good())
		std::cerr << "Warning : failure at save while writing in file\n";
}

void ImagePPM::map(void (*func)(OCTET&)) {
	
	for (OCTET& o : m_red)
		func(o);
	
	for (OCTET& o : m_green)
		func(o);

	for (OCTET& o : m_blue)
		func(o);
}

void ImagePPM::map(void (*func)(const OCTET&)) const {
	
	for (const OCTET& o : m_red)
		func(o);
	
	for (const OCTET& o : m_green)
		func(o);

	for (const OCTET& o : m_blue)
		func(o);
}

void seuillage(ImagePGM& image, const std::vector<int>& seuils){
  
  bool assigned = false;
  
  if(seuils.size() > 0) {

    for (size_t i = 0; i < image.height(); ++i){
      for (size_t j = 0; j < image.width(); ++j){
        
        assigned = false;

        for (size_t k = 0; k < seuils.size(); ++k){
          if (image(i, j) < seuils[k]){
            image(i, j) = (255 / seuils.size()) * k ;
            assigned = true;
            break;
          }
        }

        if (!assigned)
          image(i, j) = 255;

      }
    }

  }
}

std::vector<int> histo_grey(const ImagePGM& image) {

	std::vector<int> histo(256);

	for (size_t i = 0; i < image.height(); ++i){
		for (size_t j = 0; j < image.width(); ++j){
			histo[image(i, j)]++;
		}
	}

	return std::move(histo);
}


std::vector<std::array<int, 3> > histo_color(const ImagePPM& image) {

  std::vector<std::array<int, 3> > RGB_occurrences(256);

  for (size_t i = 0; i < image.height(); ++i){
    for (size_t j = 0; j < image.width(); ++j){
      RGB_occurrences[image.red(i, j)][0]++;
      RGB_occurrences[image.green(i, j)][1]++;
      RGB_occurrences[image.blue(i, j)][2]++;
    }
  }

  return std::move(RGB_occurrences);
}

void print_histo_grey(std::vector<int> histo_grey) {

	//double nb_pixels = image.height() * image.width();

	for (size_t i = 0 ; i < histo_grey.size() ; ++i){
		std::cout << i << " " << histo_grey[i] << "\n";
	}
}

void print_histo_color(std::vector<std::array<int, 3> > histo_color) {

	//double nb_pixels = image.height() * image.width();

	for (size_t i = 0 ; i < histo_color.size() ; ++i){
		std::cout << i << " " << histo_color[i][0] << " " << histo_color[i][1] << " " << histo_color[i][2] << "\n";
	}
}

void print_histo_grey(std::vector<int> histo_grey, long nb_pixels) {

	for (size_t i = 0 ; i < histo_grey.size() ; ++i){
		std::cout << i << " " << histo_grey[i] / (double)nb_pixels << "\n";
	}
}

void print_histo_color(std::vector<std::array<int, 3> > histo_color, long nb_pixels) {

	for (size_t i = 0 ; i < histo_color.size() ; ++i){
		std::cout << i << " " << histo_color[i][0] / (double)nb_pixels << " " << histo_color[i][1] / (double)nb_pixels << " " << histo_color[i][2] / (double)nb_pixels<< "\n";
	}
}

bool has_suffix(const std::string &str, const std::string &suffix){
	return str.size() >= suffix.size() && 
		   str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}


void min_max_grey(const std::vector<int>& histo, int& min, int& max) {

	for (size_t i = 0; i < histo.size(); ++i){
		if (histo[i] > 0) {
			min = i;
			break;
		}
	}
	
	for (size_t i = histo.size() - 1 ; i > 0; --i){
		if (histo[i] > 0) {
			max = i;
			break;
		}
	}
}

void expansion(ImagePGM& image, int min, int max) {
	for (OCTET& o : image)
		o = (OCTET)round(255.0 / ((double)(max - min)) * ((double)(-min + o)));
}

std::string remove_extension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    return lastdot == std::string::npos ? filename : filename.substr(0, lastdot); 
}

void seuillage_extrema(ImagePGM& image, int min, int max) {
	for (OCTET& o : image) {
		if (o > max) {
			o = max;
		} else if (o < min) {
			o = min;
		}
	}
}

std::vector<double> ddp_grey(const std::vector<int>& histo, long nb_pixels) {

	std::vector<double> ddp(histo.size());

	for (size_t i = 0 ; i < histo.size() ; ++i){
		ddp[i] = histo[i] / (double)nb_pixels;
	}

	return std::move(ddp);
}


void print_ddp_grey(const std::vector<double>& ddp) {
	for (size_t i = 0; i < ddp.size(); ++i) {
		std::cout << i << " " << ddp[i] << "\n"; 
	}
}

std::vector<double> repartition_grey(const std::vector<double>& ddp) {
	
	std::vector<double> repartition(ddp.size());

	if (ddp.empty()) {
		std::cerr << "Error : ddp is empty\n";
		return std::move(repartition);
	}

	repartition[0] = ddp[0];

	for (size_t i = 1 ; i < ddp.size() ; ++i){
		repartition[i] = repartition[i-1] + ddp[i];
	}

	return std::move(repartition);
}

void egalisation(ImagePGM& image, std::vector<double> repartition) {
	for (OCTET& o : image)
		o = repartition[o] * 255;
}


double inverse_value(const std::vector<double>& repartition, int index) {
 
	int k = 0;
	
	while (k < 255) {
		
		if (repartition[k] >= (double)index)
			break;
		
		k++;
	}

	if (k == 255) 
		std::cerr << "For index " << index << ", k was not found.\n";
  
	return (double)k;
}

std::vector<double> repartition_inverse_grey(const std::vector<double>& repartition) {
	
	std::vector<double> repartition_inv(repartition.size());

	for (size_t i = 1 ; i < repartition.size() ; ++i){
		repartition_inv[i] = inverse_value(repartition, i);
	}

	return std::move(repartition_inv);
}