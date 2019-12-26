#include <stdio.h>
#include <iostream>
#include <vector>
#include "../librairie/custom_c++/ImageBase.h"

using namespace std;

bool on_bound(const ImagePGM& image, int raw, int column) {
  return raw >= 0 && raw < image.height() && column >= 0 && column < image.width(); 
}

void assign_voisins_4(ImagePGM& image, int raw, int column, int value){
  if (on_bound(image, raw + 1, column))
    image[raw + 1][column] = value;

  if (on_bound(image, raw - 1, column))
    image[raw - 1][column] = value;

  if (on_bound(image, raw, column + 1))
    image[raw][column + 1] = value;

  if (on_bound(image, raw, column - 1))
    image[raw][column - 1] = value;
}

void assign_voisins_8(ImagePGM& image, int raw, int column, int value){
  assign_voisins_4(image, raw, column, value);

  if (on_bound(image, raw + 1, column + 1))
    image[raw + 1][column + 1] = value;

  if (on_bound(image, raw + 1, column - 1))
    image[raw + 1][column - 1] = value;

  if (on_bound(image, raw - 1, column + 1))
    image[raw - 1][column + 1] = value;

  if (on_bound(image, raw - 1, column - 1))
    image[raw - 1][column - 1] = value;  
}

void assign_voisins(ImagePGM& image, int raw, int column, int value, bool are_vois_8){
  if (are_vois_8)
    assign_voisins_8(image, raw, column, value);
  else
    assign_voisins_4(image, raw, column, value);
}

void dilatation(const ImagePGM& image_in, ImagePGM& image_out, bool are_vois_8) {
  for (int i = 0; i < image_in.height(); ++i) {
    for (int j = 0; j < image_in.width(); ++j) {
      if (image_in[i] == 0)
        assign_voisins(image_out, i, j, 0, are_vois_8);
    }
  }
}

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: <ImageIn.pgm> <ImageOut.pgm> <BOOL:Voisins8>\n");
    printf("Attention a faire en sorte que SN < SN+1.\n");
    return 1;
  }

  vector<int> seuils;

  for (int i = 3; i < argc; ++i) {
    seuils.push_back(atoi(argv[i]));
  }

  ImagePGM image_in;
  image_in.load(argv[1]);

  ImagePGM image_out = image_in;

  bool are_vois_8 = argc > 3 ? true : false;
  
  dilatation(image_in, image_out, are_vois_8);

  image_out.save(argv[2]);

  return 0;
}
