#include <stdio.h>
#include <iostream>
#include <vector>
#include "ImageBase.h"

using namespace std;

bool out_of_range_col(const ImageBase& image, int index){
  return index < 0 || index >= image.width();  
}

bool out_of_range_raw(const ImageBase& image, int index) {
  return index < 0 || index >= image.height();
}

int main(int argc, char **argv) {

  if (argc != 4) {
    cerr << "Usage : <STRING:image.pgm> <STRING:col/raw> <NOMBRE:indice>\n";
    return 1;
  }

  ImagePGM image;
  image.load(argv[1]);
  string profil_type(argv[2]);
  int index = atoi(argv[3]);

  if (profil_type == "col") {

      if (!out_of_range_col(image, index))
        for (int i = 0; i < image.height(); ++i)
          cout << i << " " << (int)image[i][index] << "\n";
      else
        cerr << "Erreur : Index out of range\n";

  } else if (profil_type == "raw") {

      if (!out_of_range_raw(image, index))
        for (int j = 0; j < image.width(); ++j)
          cout << j << " " << (int)image[index][j] << "\n";
      else
        cerr << "Erreur : Index out of range\n";

  } else {
    cerr << "Erreur : veuillez choisir entre 'col' et 'raw'\n";
    return 1;
  }

  return 0;
}
