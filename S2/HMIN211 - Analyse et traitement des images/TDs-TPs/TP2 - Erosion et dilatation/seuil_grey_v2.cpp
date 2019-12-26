#include <stdio.h>
#include <iostream>
#include <vector>
#include "../librairie/ImageV2/Image.h"

using namespace std;

void seuillage_v2(ImagePGM& image, const vector<int>& seuils){
  
  bool assigned = false;
  
  if(seuils.size() > 0) {

    for (size_t i = 0; i < image.height(); ++i){
      for (size_t j = 0; j < image.width(); ++j){
        
        assigned = false;

        for (size_t k = 0; k < seuils.size(); ++k){
          if (image(i,j) < seuils[k]){
            image(i,j) = (255 - (255 / seuils.size()) * k) ;
            assigned = true;
            break;
          }
        }

        if (!assigned)
          image(i, j) = 0;
      }
    }

  }
}

int main(int argc, char **argv) {

  if (argc < 3) {
    printf("Usage: <ImageIn.pgm> <S1 S2 ... SN>\n");
    printf("Attention a faire en sorte que SN < SN+1.\n");
    return 1;
  }

  vector<int> seuils;
  
  for (int i = 2; i < argc; ++i) {
      seuils.push_back(atoi(argv[i]));
  }

  ImagePGM image(argv[1]);

  seuillage_v2(image, seuils);

  cout << image;

  return 0;
}
