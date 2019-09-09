#include <iostream> 
#include <cmath>
#include "Image.h"

using namespace std;

void bresenham_line_algo(ImagePGM& image, int x0, int y0, int x1, int y1, OCTET color = 255)
{
	int dx = x1 - x0;
	int dy = y1 - y0;

	int innerHor = 2 * dy;
	int innerDiag = 2 * (dx - dy);

	int e = 2 * dy - dx;

	for (int x = x0, y = y0 ; x <= x1 ; ++x)
	{
		image(y, x) = color;
		
		if (e >= 0)
		{
			y++;
			e -= innerDiag;
		}
		else
		{
			e += innerHor;
		}
	}
}

void reveilles_line_algo(ImagePGM& image, int x0, int y0, int x1, int y1, OCTET color = 255)
{
	int dx = x1 - x0;
	int dy = y1 - y0;

	int u = dy * x0 - dx * y0;
	int r = dy * x0 - dx * y0 - u;
	
	int x = x0;
	int y = y0;

	image(y, x) = color;

	while (x < x1)
	{
		x++;
		r += dy;

		if (r < 0 || r >= dx)
		{
			y++;
			r -= dx;
		}

		image(y, x) = color;
	}
}

void bresenham_line(ImagePGM& image, int x0, int y0, int x1, int y1, OCTET color = 255)
{
	int min_x = min(x0, x1);
	int max_x = max(x0, x1);

	int min_y = min(y0, y1);
	int max_y = max(y0, y1);

	int x = min_x;
	int y = min_y;

	// Calcule de la pente en abscisse et en ordonnee.

	int dx = x1 - x0;
	int dy = y1 - y0;

	// Calcule du decalage en fonction d'un deplacement vertical/horizontal

	int horizontal_offset = 2 * dx;
	int vertical_offset = 2 * dy;

	// Quadrant 1

	if (dx >= dy)
	{
		// Octant 1
		
		int error = dx;

		while (x < max_x)
		{
			image(y, x) = color;

			x++; // Deplacement horizontal
			error -= vertical_offset;

			if (error <= 0)
			{
				y++; // Deplacement vertical
				error += horizontal_offset;
			}
		}
	}
	else
	{
		// Octant 2

		int error = dy;

		while (y < max_y)
		{
			image(y, x) = color;

			y++; // Deplacement vertical
			error -= horizontal_offset;

			if (error <= 0)
			{
				x++; // Deplacement horizontal
				error += vertical_offset;
			}
		}
	}

	image(y, x) = color; // affiche le dernier point
}

void thick_line(ImagePGM& image, int x0, int y0, int x1, int y1, int width, OCTET color = 255)
{
	for (int i = 0 ; i < width ; ++i)
	{
		bresenham_line(image, x0, y0 + i, x1, y1 + i, color);
	}
}

void to_lower(string& str)
{
	for (char& c : str)
		c = std::tolower(c);
}
// Dessine un ligne du point (x1, y1) jusqu'au point (x2, y2)
void draw_line(ImagePGM& image, int x1, int y1, int x2, int y2, int intensity = 255)
{
	int xdiff = std::max(x1, x2) - std::min(x1, x2);
	int ydiff = std::max(y1, y2) - std::min(y1, y2);
	int xdir = x1 <= x2 ? 1 : -1;
	int ydir = y1 <= y2 ? 1 : -1;

	int r = std::max(xdiff, ydiff);

	for (int i = 0; i < r + 1; ++i)
	{
		int x = x1;
		int y = y1;

		if (ydiff)
			y += (float(i) * ydiff) / r * ydir;
		if (xdiff)
			x += (float(i) * xdiff) / r * xdir;

		image(y, x) = intensity;
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 5)
	{
		cerr << "Usage : " << argv[0] << " <X0 Y0 X1 Y1> [BRESENHAM|REVEILLES|THICK] <WIDTH>\n";
		exit(EXIT_FAILURE);
	}

	int x0 = atoi(argv[1]);
	int y0 = atoi(argv[2]);
	int x1 = atoi(argv[3]);
	int y1 = atoi(argv[4]);

	int width = 1;

	string algo = "bresenham";

	if (argc > 5)
		algo = argv[5];

	to_lower(algo);

	if (argc > 6)
		width  = atoi(argv[6]);
	
	ImagePGM out(100, 100);

	if (algo == "reveilles")
	{
		reveilles_line_algo(out, x0, y0, x1, y1);

		if (argc > 6)
			reveilles_line_algo(out, x0, y0 + width, x1, y1 + width);
	}
	else if (algo == "thick")
	{
		thick_line(out, x0, y0, x1, y1, width);
	}
	else
	{
		bresenham_line(out, x0, y0, x1, y1);

		if (argc > 6)
			bresenham_line(out, x0, y0 + width, x1, y1 + width);
	}

	cout << out;

	return 0;
}

