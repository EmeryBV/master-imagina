# Visualisation Volumique (Odorico Thibault)

## Lecture est stockage d'image 3D

les images 3D de ce TP sont une suite d'image 2D stocké de manière contigu dans un fichier avec chaque pixel codé sur 2 bytes (unsigned short)

### Chargement en mémoire

- image 3D : tableau d'image 2D de taille dimZ.
- image 2D : tableau de pixel de taille dimX * dimY.

### Code

```c++
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// Definition des structures

typedef unsigned short PIXEL;
typedef vector<PIXEL> IMAGE_2D;
typedef vector<IMAGE_2D> IMAGE_3D;

// Creation d'une image 2D a partir d'un fichier

std::istream& read(std::istream& is, IMAGE_2D& image2d, int dim_x, int dim_y)
{
	image2d.resize(dim_x * dim_y);

	for (PIXEL& p : image2d)
	{
		unsigned char bytes[2];
		is.read((char*)bytes, sizeof(bytes)); // lecture de 2 octets
		p = (bytes[0] << 8 | bytes[1]); // conversion en unsigned short
	}

	return is;
}

// Creation d'une image 3D a partir d'un fichier

std::istream& read(std::istream& is, IMAGE_3D& image3d, int dim_x, int dim_y, int dim_z)
{
	image3d.resize(dim_z);

	for (IMAGE_2D& image2d : image3d)
	{
		read(is, image2d, dim_x, dim_y);
	}

	return is;
}

// Accesseur au pixel de l'image 2d

PIXEL pixel(const IMAGE_2D& image2d, int i, int j, int dim_x, int dim_y)
{
	// on utillise ((dim_y - 1) - j) car les pixel de chaque ligne sont sauvegardé à l'inverse du sens lu. 
	return image2d[((dim_y - 1) - j) * dim_x + i];
}

// Accesseur au voxel de l'image 3d

PIXEL voxel(const IMAGE_3D& image3d, int i, int j, int k, int dim_x, int dim_y)
{
	return pixel(image3d[k], i, j, dim_x, dim_y);
}

PIXEL min(const IMAGE_3D& image3d)
{
	PIXEL min = numeric_limits<PIXEL>::max();

	for (const IMAGE_2D& image2d : image3d)
	{
		auto current_min = min_element(image2d.begin(), image2d.end());
		
		if (*current_min < min)
			min = *current_min;
	}

	return min;
}

PIXEL max(const IMAGE_3D& image3d)
{
	PIXEL max = numeric_limits<PIXEL>::min();

	for (const IMAGE_2D& image2d : image3d)
	{
		auto current_max = max_element(image2d.begin(), image2d.end());
		
		if (*current_max > max)
			max = *current_max;
	}

	return max;
}

// Ecriture d'une image 2D dans un flux de sortie

std::ostream& write(std::ostream& os, const IMAGE_2D& image2d)
{
	for (const PIXEL& p : image2d)
	{
		unsigned char bytes[2];
		memcpy(&p, bytes, sizeof(p)); // on copy le pixel dans bytes pour le mettre sur 2 octets
		os.write(bytes, sizeof(bytes));
	}

	return os;
}

// Ecriture d'une image 3D dans un flux de sortie

std::ostream& write(std::ostream& os, const IMAGE_3D& image3d)
{
	for (const IMAGE_2D& image2d : image3d)
		write(os, image2d);

	return os;
}

int main(int argc, char** argv)
{
	if (argc < 5)
	{
		cerr << "Usage : " << argv[0] << " <image_file> <dim_x> <dim_y> <dim_z> <x> <y> <z>\n";
		exit(EXIT_FAILURE);
	}

	ifstream image_file(argv[1], std::ios::binary);

	if (!image_file.is_open())
	{
		cerr << "Erreur à l'ouverture du fichier\n";
		exit(EXIT_FAILURE);
	}
	
	int dim_x = atoi(argv[2]);
	int dim_y = atoi(argv[3]);
	int dim_z = atoi(argv[4]);

	int x = 0;
	int y = 0;
	int z = 0;
	
	if (argc > 7)
	{
		x = atoi(argv[5]);
		y = atoi(argv[6]);
		z = atoi(argv[7]);
	}

	IMAGE_3D image3d;

	read(image_file, image3d, dim_x, dim_y, dim_z);

	// Test de l'image

	cout << "min : " << min(image3d) << "\n";
	cout << "max : " << max(image3d) << "\n";

	if (argc > 7)
		cout << voxel(image3d, x, y, z, dim_x, dim_y) << "\n";

	// Ecriture de l'image dans la sortie standard

	write(cout, image3d[z]);

	return 0;
}
```

## Affichage en Volume

Je n'ai pas eu le temps de réaliser cette exercice.