#include <stdio.h>
#include <iostream>
#include <vector>
#include "ImageBase.h"

using namespace std;
/*
#define FOND 255
#define OBJET 0

bool on_bound(const ImagePGM& image, int raw, int column) {
  return raw >= 0 && raw < image.height() && column >= 0 && column < image.width(); 
}

bool is_fond(int value){
  return value == FOND;
}

bool is_objet(int value){
  return value == OBJET;
}

void difference(ImagePGM& image_in, ImagePGM& image_dilatee, ImagePGM& image_out) {
  for (int i = 0; i < image_in.height(); ++i) {
    for (int j = 0; j < image_in.width(); ++j) {
      if (is_fond(image_in[i][j]) && is_fond(image_dilatee[i][j]))
        image_out[i][j] = FOND;
      else if (is_objet(image_in[i][j]) && is_objet(image_dilatee[i][j]))
        image_out[i][j] = FOND;
      else if (is_objet(image_dilatee[i][j] && is_fond(image_in[i][j])))
        image_out[i][j] = OBJET;
    }
  }
}

*/

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: <ImageIn.pgm> <ImageOut.pgm>\n");
    return 1;
  }

  vector<int> seuils;

  for (int i = 3; i < argc; ++i) {
    seuils.push_back(atoi(argv[i]));
  }

  ImagePGM image_in(argv[1]);
  //image_in.load(argv[1]);
  ImagePGM image_out(image_in);

  //image_dilatee.load(argv[2]);


  //difference(image_in, image_dilatee, image_out);

  image_out.save(argv[2]);

  return 0;
}