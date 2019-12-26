#include <iostream>
#include <type_traits>
#include <cassert>
#include <tuple>

#include "static_multidim.hpp"
// #include "Image.h"

using namespace std;

using Matrix4x4 = Multidim_array<int, 4, 4>;



// auto multiply = [](auto&&... xs) { return (std::forward<decltype(xs)>(xs) * ...); };

// auto multiply = [](auto left, auto right,) { return left * right; };
/*
template<size_t DIM_SIZE, size_t... DIM_SIZE_MORE>
constexpr size_t calc_dim()
{
	return std::apply([](auto&&... xs){
				return (... & xs);
			},
			make_tuple(DIM_SIZE, DIM_SIZE_MORE...));
}
*/

using Matrix = std::array<std::array<int, 3>, 2>;

int main(int argc, char** argv)
{
	if (argc < 1)
    {
    	cerr << "Usage : " << argv[0] << "\n";
		return EXIT_FAILURE;
	}

	Multidim_array<int, 2, 3> array; // = {{1, 2, 3}, {4, 5, 6}};

	// cerr << "array[0].size() = " << array[1].size() << "\n";
	// Multidim_array<int, 5> array;
	
/*
	fill(array.begin(), array.end(), 0);

	for (size_t i = 0 ; i < array.size() ; ++i)
	{
		cerr << "array[" << i << "] = " << array[i] << "\n"; 
	}
*/
	// auto tuple = make_tuple();
	// cerr << "dim = " << calc_dim<1>() << "\n";

	// std::array<int, 3> a = {1, 2, 3};
	// std::array<int, 3> b = {4, 5, 6};
	// std::array<int, 3> c = {7, 8, 9};

/*
	Multidim_array<int, 3, 3, 3> matrix
	{{
		{{
			{1 , 2 , 3 },
			{4 , 5 , 6 },
			{7 , 8 , 9 }
		}},
		{{ 
			{10, 11, 12},
			{13, 14, 15},
			{16, 17, 18}
		}},
		{{
			{19, 20, 21},
			{22, 23, 24},
			{25, 26, 27}
		}}
	}};
	*/
	// Matrix3x3 matrix = {a, b, c};

	// matrix(1, 2, 3) = 4;

	// fill(matrix.begin(), matrix.end(), 0);

	// for (size_t i = 0 ; i < matrix.size() ; ++i)
	// {
		// cerr << "matrix[" << i << "] = " << array[i] << "\n"; 
	// }
	// Matrix4x4 matrix = {0, 1, 2, 3};

	//indexed(z, y,  x...);
	// cerr << "matrix(z, y, x) = " << matrix(0, 1, 2) << "\n";
	// cerr << typeid(matrix).name() << "\n";

	return EXIT_SUCCESS;
}

/*
faculté des sciences / relation internationales
choisir destination > aller sur site département informatique rebrique erasmus / internationale
1 mois pour se décider
Bourses etrangère pour les non boursier boursier du crous : FORFAIT RÉGIONAL INDIVIDUALISÉ
 */