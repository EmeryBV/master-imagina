#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <array>
#include "../lib/Image.h"

using namespace std;

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> v)
{
	for (size_t i = 0; i < v.size(); ++i)
	{
		os << v[i] << "\n";
	}

	return os;
}

bool is_prime(size_t x)
{
	if (x < 2) 
		return false;
	for(size_t i = 2 ; i <=  sqrt(x); i++)
		if ((x % i) == 0) 
			return false;
	return true;
}

size_t gcd_extended(size_t a, size_t b, size_t& x, size_t& y)  
{  
	if (a == 0)  
	{  
		x = 0;
		y = 1;
		return b;
	}  

	size_t x_rec, y_rec;
	size_t gcd = gcd_extended(b%a, a, x_rec, y_rec);  

	x = y_rec - (b / a) * x_rec;  
	y = x_rec;  

	return gcd;  
}

size_t gcd(size_t a, size_t b)
{
	size_t x, y;
	return gcd_extended(a, b, x, y);
}

bool coprime(size_t a, size_t b)
{
	return gcd(a, b) == 1;
}

// Valeur maximale avec max = -1
void print_primes(size_t min = 2, size_t max = -1)
{
	for (size_t i = min; i < max; ++i)
	{
		if (is_prime(i))
		{
			std::cout << i << "\n";
		}
	}
}

std::vector<size_t> exposants_chiffrement(size_t a, size_t b)
{
	// T n = a * b;
	size_t phi_n = (a - 1) * (b - 1);

	std::vector<size_t> exposants;
	
	for (size_t i = 2; i < phi_n; ++i)
		if(coprime(i, phi_n))
			exposants.push_back(i);

	return exposants;
}

// Renvoie l'exponentiation modulaire de base
template<typename T>
T modpow(T base, T exp, T modulo)
{
	T result = 1;

	while (exp > 0)
	{
		if ((exp & 1) > 0)
			result = (result * base) % modulo;
		
		exp >>= 1;
		base = (base * base) % modulo;
	}

	return result;
}

ImagePGM chiffrement_rsa(const ImagePGM& image, size_t prime_p, size_t prime_q, size_t exposant)
{
	ImagePGM image_rsa = image;

	for (auto& px : image_rsa)
	{
		// px = clamp(modpow<size_t>(px, exposant, prime_product), (size_t)0, (size_t)255); 
		px = modpow<size_t>(px, exposant, prime_p * prime_q); 
	}

	return std::move(image_rsa);
}

// Renvoie l'inverse modulaire de n
size_t mod_inv(size_t n, size_t m) 
{ 
    size_t x, y; 
    size_t g = gcd_extended(n, m, x, y); 
    
    if (g != 1) // l'exponentiation modulaire existe !
		std::cerr << "L'inverse modulaire pour ce nombre n'existe pas !\n";
    /*
    {
    	result = (x % m + m) % m;
    	return true;
    }
	*/
    return (x % m + m) % m;
} 

ImagePGM dechiffrement_rsa(const ImagePGM& image_rsa, size_t prime_p, size_t prime_q, size_t exposant)
{
	ImagePGM image = image_rsa;

	size_t phi_n = (prime_p - 1) * (prime_q - 1);
	size_t exposant_inv = mod_inv(exposant, phi_n);

	std::cerr << "exposant_inv = " << exposant_inv << "\n";

	for (auto& px : image)
	{
		// std::cerr << "Pas de prime_product pour ce pixel !\n";

		// px = clamp(modpow<size_t>(px, exposant_inv, prime_product), (size_t)0, (size_t)255); 
		px = modpow<size_t>(px, exposant_inv, prime_p * prime_q); 
	}

	return std::move(image);
}


double entropie_shannon(const ImagePGM& image)
{
	std::array<size_t, 256> pixel_occurence = {0}; // remplie de 0

	double nb_pixels = image.size();

	for(const OCTET pixel : image)
	{
		pixel_occurence[pixel]++; 
	}

	double sum = 0;

	for(auto occurence : pixel_occurence)
	{
		double P_pixel = occurence / nb_pixels;

		if (occurence != 0)
			sum += P_pixel * log2(P_pixel);
	}

	return -sum;
}

void tests()
{
	cerr << "Debut des tests unitaires...\n";
	assert(is_prime(11));
	assert(is_prime(23));
	assert(coprime(11, 23));
	cerr << "Tests tous passés avec succès !\n";
}

int main(int argc, char** argv)
{
	if (argc < 3)
    {
    	cerr << "Usage : " << argv[0] << " <\"chiff\"/\"dechiff\"> <image_in.pgm>\n";
    	cerr << "\n";
    	cerr << "Pour recuper une image en sortie il faudra faire une redirection de flux.\n";
    	cerr << "L'image d'entrée ne doit pas être la même que celle de sortie.\n";
    	cerr << "Exemple:\n";
    	cerr << argv[0] << " image.pgm chiff > image_chiffree.pgm:\n";
		return EXIT_FAILURE;
	}


	string mode(argv[1]);

	assert(mode == "chiff" || mode == "dechiff");

	ImagePGM image_in;

	cerr << "Lecture de l'image...\n";

	image_in.load(argv[2]);


	size_t exposant = 17;
	size_t prime_p = 11;
	size_t prime_q = 23;

	cerr << "exposant = " << exposant << "\n";
	cerr << "prime_p = " << prime_p << "\n";
	cerr << "prime_q = " << prime_q << "\n";
	
	ImagePGM image_out;
	
	if (mode == "chiff")
	{
		cerr << "Chiffrement de l'image...\n";
		image_out = chiffrement_rsa(image_in, prime_p, prime_q, exposant);
	}
	else if (mode == "dechiff")
	{
		cerr << "Déchiffrement de l'image...\n";
		image_out = dechiffrement_rsa(image_in, prime_p, prime_q, exposant);
	}



	cerr << "Calcul de l'entrepie de l'image...\n";

	cerr << "entropie_shannon(image_out) = " << entropie_shannon(image_out) << " bpp\n";

	cerr << "Ecriture de l'image...\n";

	cout << image_out;

	return 0;
}
