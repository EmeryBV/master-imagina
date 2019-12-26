#ifndef IMAGE_BASE_H
#define IMAGE_BASE_H

#include <iostream>
#include <vector>

#include "image_pgm_ppm.h"

class ImageBase {

protected:

	std::vector<OCTET> m_data;

	int m_height;
	int m_width;

	bool m_is_valid;

	ImageBase(int width, int height, size_t data_size);

public:

	ImageBase();
	ImageBase(int width, int height);

	int height() const { return m_height; };
	int width() const { return m_width; };
	int data_size() const { return m_data.size(); };
	int is_valid() const { return m_is_valid; };

	OCTET* data() { return m_data.data(); };
	const OCTET* data() const { return m_data.data(); };

	OCTET* operator[](int raw);

	// ajouter iterateurs :
	// begin() end()
	
	// charge une image en fonction de sont type (.ppm / .pgm)
	virtual bool load(const std::string& filename) = 0;
	virtual bool save(const std::string& filename) = 0;
};

////////////////////////////////// ImagePGM ////////////////////////////////////////

class ImagePGM : public ImageBase {
public:

	ImagePGM() : ImageBase() {}
	ImagePGM(int width, int height) : ImageBase(width, height) {}

	bool load(const std::string& filename);
	bool save(const std::string& filename);
};

////////////////////////////////// ImagePPM ////////////////////////////////////////

class ImagePPM : public ImageBase {

public:

	typedef enum { PLAN_R, PLAN_G, PLAN_B} PLAN;

	ImagePPM() : ImageBase() {}
	ImagePPM(int width, int height);
	ImagePPM(const ImagePGM& red, const ImagePGM& green, const ImagePGM& blue);

	bool load(const std::string& filename);
	bool save(const std::string& filename);

	ImagePGM get_plan(PLAN);
	void set_plan(PLAN, const ImagePGM& grey_image);

	// ajouter iterateurs :
	// red_begin() red_end()
	// green_begin() green_end()
	// blue_begin() blue_end()
};

////////////////////////////////// FONCTIONS ////////////////////////////////////////

/* Assigne les données du plan à l'image coloré, dont la taille doit être 3 fois superieur à celle du plan */
void set_plan_R(OCTET *image_data, const OCTET *plan_R, int taille_plan);
void set_plan_G(OCTET *image_data, const OCTET *plan_G, int taille_plan);
void set_plan_B(OCTET *image_data, const OCTET *plan_B, int taille_plan);

#endif