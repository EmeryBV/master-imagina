#include <iostream>
#include <random>
#include "../Matrix.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
	// TODO : Gerer les problèmes d'overflow pendant la multiplication de matrix

	Matrix<int> A = {{1, 2, 3}};
	
	cerr << "A:\n";
	cerr << A;

	Matrix<int> B = {{1},
                     {2},
                     {3}};

	cerr << "B:\n";
	cerr << B;

    Matrix<int> C = {{1,  2,  3},
                     {4,  5,  6},
                     {7,  8,  9}};
	cerr << "C:\n";
	cerr << C;

	// assert(A.same_dimension(C));
	assert(B.width() == 1);
	assert(B.height() == 3);

	// Matrix<int> product =;

	cerr << "C * B:\n";
	cerr <<  C * B;



	return 0;
} 
