#include <stdio.h>
#include <iostream>
#include <vector>
#include "../librairie/ImageV2/Image.h"

using namespace std;


void assign_voisins_4(ImagePGM& image, int row, int column, int value){
  if (image.on_bounds(row + 1, column))
    image(row + 1, column) = value;

  if (image.on_bounds(row - 1, column))
    image(row - 1, column) = value;

  if (image.on_bounds(row, column + 1))
    image(row, column + 1) = value;

  if (image.on_bounds(row, column - 1))
    image(row, column - 1) = value;
}

void assign_voisins_8(ImagePGM& image, int row, int column, int value){
  assign_voisins_4(image, row, column, value);

  if (image.on_bounds(row + 1, column + 1))
    image(row + 1, column + 1) = value;

  if (image.on_bounds(row + 1, column - 1))
    image(row + 1, column - 1) = value;

  if (image.on_bounds(row - 1, column + 1))
    image(row - 1, column + 1) = value;

  if (image.on_bounds(row - 1, column - 1))
    image(row - 1, column - 1) = value;  
}

void assign_voisins(ImagePGM& image, int row, int column, int value, bool are_vois_8){
  if (are_vois_8)
    assign_voisins_8(image, row, column, value);
  else
    assign_voisins_4(image, row, column, value);
}

void fermeture(ImagePGM& image_in, ImagePGM& image_out, int value, bool are_vois_8) {
  for (size_t i = 0; i < image_in.height(); ++i) {
    for (size_t j = 0; j < image_in.width(); ++j) {
      if (image_in(i, j) == value)
        assign_voisins(image_out, i, j, value, are_vois_8);
    }
  }
}

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: <ImageIn.pgm> <String:Dilatation/Erosion> <NB_VOIS> <NB_EXEC> \n");
    printf("On peut appliquer plusieurs errosion à la suite avec EE...\n");
    printf("On peut appliquer plusieurs dilatation à la suite avec DD...\n");
    printf("On peut alterner EDEED...\n");
    return 1;
  }

  vector<int> seuils;

  for (int i = 2; i < argc; ++i) {
    seuils.push_back(atoi(argv[i]));
  }

  ImagePGM image_in;
  image_in.load(argv[1]);

  ImagePGM image_out = image_in;

  //E = errosion et D = dilatation
  string arg_fermeture(argv[2]);

  bool are_vois_8 = false;

  int nb_exec = 1;

  if (argc >= 4)
    are_vois_8 = atoi(argv[3]) == 8 ? true : false;
  
  if (argc >= 5)
    nb_exec = atoi(argv[4]);

  for (int k = 0; k < nb_exec; ++k){
    for (size_t i = 0; i < arg_fermeture.size(); ++i){

      if (arg_fermeture[i] == 'E'){
        fermeture(image_in, image_out, 255, are_vois_8);
      } else if (arg_fermeture[i] == 'D') {
        fermeture(image_in, image_out, 0, are_vois_8);
      } else {
        cerr << "Erreur : l'argument doit etre E ou D\n";
      } 

      image_in = image_out;
    }
  }
  
  cout << image_out;

  return 0;
}