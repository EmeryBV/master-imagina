#include <stdio.h>
#include <iostream>
#include <vector>
#include "ImageBase.h"

using namespace std;

void seuillage(ImagePGM& image, const vector<int>& seuils){
  
  bool assigned = false;
  
  if(seuils.size() > 0) {

    for (int i = 0; i < image.height(); ++i){
      for (int j = 0; j < image.width(); ++j){
        
        assigned = false;

        for (size_t k = 0; k < seuils.size(); ++k){
          if (image[i][j] < seuils[k]){
            image[i][j] = (255 / seuils.size()) * k ;
            assigned = true;
            break;
          }
        }

        if (!assigned)
          image[i][j] = 255;

      }
    }

  }
}

int main(int argc, char **argv) {

  if (argc != 3) {
    printf("Usage: <ImageIn.pgm> <ImageOut.pgm>\n");
    printf("Prend en flux le resultat de histo_grey\n");
    return 1;
  }

  int indice;
  double occurence;
  double sum_occ = 0; 

  while(sum_occ < 0.5 && cin >> indice >> occurence) {
  	sum_occ += occurence;
  }

  vector<int> seuils = {indice};

  cout << "Seuil trouvé : " << indice << "\n";
  
  for (int i = 3; i < argc; ++i) {
      seuils.push_back(atoi(argv[i]));
  }

  ImagePGM image; image.load(argv[1]);

  seuillage(image, seuils);

  image.save(argv[2]);

  return 0;
}
