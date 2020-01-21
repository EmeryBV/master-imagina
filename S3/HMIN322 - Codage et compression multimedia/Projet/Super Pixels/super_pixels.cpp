#include <iostream>
#include <limits>
#include <algorithm>
#include <map>

// #include "PortableImageUtils.hpp"
#include "ScalarArray.hpp"
#include "PortableImage.hpp"
// #include "RGB.hpp"

template<typename T>
using Color3 = ScalarArray<T, 3>;
using Color3d = Color3<double>;

template<typename T>
using Vector2 = ScalarArray<T, 2>;
using Vector2u = Vector2<size_t>;
using Vector2d = Vector2<double>;


using PPMLab = GenericPPM<Color3d>;


double EQM(const PGM& left, const PGM& right)
{
	assert(left.same_dimension(right));

	double sum = 0;

	for (size_t i = 0 ; i < left.size() ; ++i)
	{
		sum += pow(left[i] - right[i], 2);
	}

	return sum / (double)left.size();
}

double PSNR(const PGM& left, const PGM& right)
{
	return 10 * log10(pow(255, 2) / EQM(left, right));
}

double EQM(const PPM& left, const PPM& right)
{
	assert(left.same_dimension(right));

	double eqm_r = 0;
	double eqm_g = 0;
	double eqm_b = 0;

	for (size_t i = 0 ; i < left.size() ; ++i)
	{
		eqm_r += pow(left[i].r() - right[i].r(), 2);
		eqm_g += pow(left[i].g() - right[i].g(), 2);
		eqm_b += pow(left[i].b() - right[i].b(), 2);
	}

	return (eqm_r + eqm_g + eqm_b) / (left.size() * 3);
}

double PSNR(const PPM& left, const PPM& right)
{
	return 10 * log10(pow(255, 2) / EQM(left, right));
}

template<typename T>
T clamp(const T& value, const T& min, const T& max)
{
	assert(min < max);
	return value < min ? min : value > max ? max : value; 
}

Color3<double> RGB_to_XYZ(const RGB& rgb)
{
	double R = rgb[0] / 255.0;
	double G = rgb[1] / 255.0;
	double B = rgb[2] / 255.0;

	double r, g, b;

	if(R <= 0.04045)	r = R/12.92;
	else				r = pow((R+0.055)/1.055,2.4);
	if(G <= 0.04045)	g = G/12.92;
	else				g = pow((G+0.055)/1.055,2.4);
	if(B <= 0.04045)	b = B/12.92;
	else				b = pow((B+0.055)/1.055,2.4);

	return Color3<double>(
		r * 0.4124564 + g * 0.3575761 + b * 0.1804375,
		r * 0.2126729 + g * 0.7151522 + b * 0.0721750,
		r * 0.0193339 + g * 0.1191920 + b * 0.9503041
	);
};

Color3<double> RGB_to_LAB(const RGB& rgb)
{

	Color3<double> xyz = RGB_to_XYZ(rgb);
	
	double X = xyz[0], Y = xyz[1], Z = xyz[2];

	double epsilon = 0.008856;	// actual CIE standard
	double kappa   = 903.3;		// actual CIE standard

	double Xr = 0.950456;	// reference white
	double Yr = 1.0;		// reference white
	double Zr = 1.088754;	// reference white

	double xr = X / Xr;
	double yr = Y / Yr;
	double zr = Z / Zr;

	double fx, fy, fz;

	if(xr > epsilon)	fx = pow(xr, 1.0/3.0);
	else				fx = (kappa*xr + 16.0)/116.0;
	if(yr > epsilon)	fy = pow(yr, 1.0/3.0);
	else				fy = (kappa*yr + 16.0)/116.0;
	if(zr > epsilon)	fz = pow(zr, 1.0/3.0);
	else				fz = (kappa*zr + 16.0)/116.0;

	return Color3<double>(
		116.0 * fy - 16.0,
		500.0 * (fx - fy),
		200.0 * (fy - fz)
	);
};

template<typename T, size_t N>
double distance(const ScalarArray<T, N>& left, const ScalarArray<T, N>& right)
{
	double sum = 0;
	for (size_t i = 0 ; i < N ; ++i)
	{
		sum += pow((left[i] - right[i]), 2);
	}
	return sqrt(sum);
}

template<typename T, size_t N>
double norm_squared(const ScalarArray<T, N>& s_array)
{
	double sum = 0;

	for (const auto& v : s_array)
	{
		sum += pow(v, 2);
	}

	return sum;
};


struct SuperPixel
{
	RGB lab;
	Vector2u xy;
	std::vector<Vector2u> pixels;
};

double super_pixel_distance(const SuperPixel& left, const SuperPixel& right, const double S, const double M)
{
	// return distance(RGB_to_LAB(left.lab), RGB_to_LAB(right.lab)) + (M / S) * distance(left.xy, right.xy);
	return distance(left.lab, right.lab) + (M / S) * distance(left.xy, right.xy);
}


PGM to_GRAY(const PPM& image)
{
	PGM gray(image.height(), image.width());
	for (size_t i = 0 ; i < gray.size() ; ++i)
	{
		double g = image[i][0] * 0.299 + image[i][1] * 0.587 + image[i][2] * 0.114;
		gray[i] = round(g);
	}

	return gray;
}

PGM gradient(const PPM& image)
{
	PGM gray = to_GRAY(image);
	PGM gradient = gray;

	for (size_t i = 1 ; i < gradient.height() - 1 ; ++i)
	{
		for (size_t j = 1 ; j < gradient.width() - 1 ; ++j)
		{
			double d = pow(gray(i, j + 1) - gray(i, j - 1), 2) + pow(gray(i + 1, j) - gray(i - 1, j), 2);
			double max_value = 2 * pow(255, 2); 
			d = 255.0 * (d / max_value);
			gradient(i, j) = clamp(round(d), 0.0, 255.0);
		}
	}

	return gradient;
};

Vector2u min_one_of_neighbors(const PGM& gradient, Vector2u ij)
{
	size_t min_i = ij[0] - 1;
	size_t min_j = ij[1] - 1;

	unsigned char current_min = gradient(min_i, min_j);

	for (size_t i = ij[0] - 1 ; i <= ij[0] + 1 ; ++i)
	{
		for (size_t j = ij[1] - 1 ; j <= ij[1] + 1 ; ++j)
		{
			if (gradient(i, j) < current_min)
			{
				min_i = i;
				min_j = j;
				current_min = gradient(i, j);
			}
		}
	}

	return Vector2u(min_i, min_j);
}

Vector2u best_matching_pixel(const PPM& image, const SuperPixel& sp, double S, double M)
{
	size_t offset = S;

	size_t best_i = std::max(round(sp.xy[0] - offset), 0.0);
	size_t best_j = std::max(round(sp.xy[1] - offset), 0.0);

	std::cerr << "best_i : " << best_i << '\n';
	std::cerr << "best_j : " << best_j << '\n';
	std::cerr << "offset : " << offset << '\n';

	SuperPixel current_pixel = { image(best_i, best_j), { best_i, best_j }, {}};

	double current_min_dist = super_pixel_distance(sp, current_pixel, S, M);


	for (size_t i = best_i ; i <= sp.xy[0] + offset ; ++i)
	{
		for (size_t j = best_j ; j <= sp.xy[1] + offset ; ++j)
		{
			size_t current_i = std::min(i, image.height());
			size_t current_j = std::min(j, image.width());
		
			SuperPixel current_pixel { image(current_i, current_j), {current_i, current_j}, {}};

			double dist = super_pixel_distance(sp, current_pixel, S, M);

			if (dist < current_min_dist)
			{
				best_i = current_i;
				best_j = current_j;
				current_min_dist = dist;
			}
		}
	}
	// std::cerr << "fin best\n";
	return Vector2u(best_i, best_j);
}

void assign_pixels_to_super_pixels(const PPM& image, std::vector<SuperPixel>& super_pixels, double S, double M)
{
	std::map<Vector2u, size_t> pixel_to_super_map;

	PGMd dist_image(image.height(), image.width(), std::numeric_limits<double>::max());

	size_t offset = S;

	for (size_t k = 0 ; k < super_pixels.size() ; ++k)
	{
		size_t best_i = std::max(round(super_pixels[k].xy[0] - offset), 0.0);
		size_t best_j = std::max(round(super_pixels[k].xy[1] - offset), 0.0);

		for (size_t i = best_i ; i <= super_pixels[k].xy[0] + offset ; ++i)
		{
			for (size_t j = best_j ; j <= super_pixels[k].xy[1] + offset ; ++j)
			{
				size_t current_i = std::min(i, image.height());
				size_t current_j = std::min(j, image.width());

				double dist = super_pixel_distance(super_pixels[k], { image(current_i, current_j), {current_i, current_j}, {} }, S, M);

				if (dist < dist_image(current_i, current_j))
				{
					Vector2u pixel = Vector2u(current_i, current_j);
					
					auto found = pixel_to_super_map.find(pixel);
					
					// si le pixel est associé a un autre super_pixel on enlève l'association
					if (found != pixel_to_super_map.end())
					{
						SuperPixel sp = super_pixels[ pixel_to_super_map[pixel] ];  // enlève le pixel de l'autre super pixel
						sp.pixels.erase(std::remove(sp.pixels.begin(), sp.pixels.end(), pixel), sp.pixels.end());
					} 
					
					pixel_to_super_map[pixel] = k;
					super_pixels[k].pixels.push_back(pixel);

					dist_image(current_i, current_j) = dist;
				}
			}
		}
	}
}

void update_super_pixels_center(const PPM& image, std::vector<SuperPixel>& super_pixels)
{
	for (auto& sp : super_pixels)
	{
		Vector2d sum;

		for (const auto& px : sp.pixels)
		{
			sum += Vector2d(px);
		}

		sum /= (double)sp.pixels.size();

		Vector2u position((size_t)round(sum[0]), (size_t)round(sum[1]));
		sp.xy = position;
		sp.lab = image(position[0], position[1]);
	}
}

void update_super_pixels_avg_color(const PPM& image, std::vector<SuperPixel>& super_pixels)
{
	for (auto& sp : super_pixels)
	{
		Color3d sum;

		for (const auto& px : sp.pixels)
		{
			RGB c = image(px[0], px[1]);
			sum += Color3d((double)c[0], (double)c[1], (double)c[2]);
		}

		sum /= (double)sp.pixels.size();

		sp.lab = RGB((unsigned char)round(sum[0]), (unsigned char)round(sum[1]), (unsigned char)round(sum[2]));
	}
}

void draw_super_pixels_color(PPM& image, const std::vector<SuperPixel>& super_pixels)
{
	for (auto& sp : super_pixels)
	{
		for (const auto& px : sp.pixels)
		{
			image(px[0], px[1]) = sp.lab;
		}
	}
}

void draw_super_pixels_center(PPM& image, const std::vector<SuperPixel>& super_pixels)
{
	for (const auto& sp : super_pixels)
	{
		image(sp.xy[0], sp.xy[1]) = RGB(0, 0, 0);
	}
}

// K : nb_super_pixels in the image
void SLIC(PPM& image, size_t K, double M = 10, size_t iterations = 5)
{
	double N = image.size(); // nb pixels
	double S = sqrt(double(N) / double(K)); // interval between super pixels centers

	size_t k = 0;
	std::vector<SuperPixel> super_pixels;
	super_pixels.reserve(K);

	// Placement des centre de super pixels sur une grille uniforme

	for (size_t i = 1 ; i < ((image.height()) / S); i++)
	{
		for (size_t j = 1 ; j < ((image.width()) / S); j++)
		{
			size_t y = (i) * S;
			size_t x = (j) * S;
			super_pixels.push_back({ image(y, x), {y, x}, {} });
			k++;
		}
	}

	// deplacement des supers pixels sur les gradient les d'intensité min
	
	PGM image_gradient = gradient(image);

	for (auto& px : super_pixels)
	{
		px.xy = min_one_of_neighbors(image_gradient, px.xy);
	}

	
	for (size_t i = 0 ; i < iterations ; ++i)
	{
		assign_pixels_to_super_pixels(image, super_pixels, S, M);
		update_super_pixels_center(image, super_pixels);
	}
	
	draw_super_pixels_color(image, super_pixels);
}

int main(int argc, char const *argv[])
{
	using namespace std;

	if (argc < 2)
    {
    	cerr << "Usage : " << argv[0] << " <image.ppm/pgm>\n";
		exit(EXIT_FAILURE);
	}

	string image_name = argv[1];

	PortableImageHeader header; header.load(image_name);

	if (is_ppm_header(header))
	{
		cerr << "Reading PPM...\n";
		PPM image_in(image_name);

		PPM image_out = image_in;

		cerr << "Application de l'algorithme SLIC...\n";
		SLIC(image_out, 2000, 10, 10);

		cerr << "PSNR(Image_in, Image_out) = " << PSNR(image_in, image_out) << "\n";

		cerr << "Writing PPM...\n";
		// cout << gradient(image_in);
		gradient(image_in).save("out.pgm");
		cout << image_out;
	}
	else
	{
		cerr << "Error : wrong image format (only PPM images supported)\n";
		exit(EXIT_SUCCESS);
	}

	return 0;
} 
