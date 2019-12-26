#include <stdio.h>
#include <iostream>
#include <vector>
#include <regex>
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

/* ajoute les seuils dans le vector seuils et renvoie l'index ou ce trouve la prochaine couleur sur laquel ajouter des seuils */
int add_seuils(int argc, const char* argv[], int index, vector<int>& seuils){
  
  while (index < argc && argv[index][0] != 'R' && argv[index][0] != 'G' && argv[index][0] != 'B') {
    seuils.push_back(atoi(argv[index]));
    index++;
  }

  return index;
}

void print_v(vector<int>& v){
  for (size_t i = 0; i < v.size(); ++i) {
    cout << v[i] << " ";
  }
  cout << "\n";
}

int main(int argc, const char *argv[]) {

  if (argc <= 3) {
    printf("Usage: <ImageIn.ppm> <ImageOut.ppm> [R <S1 S2 ... SN>] [G <S1 S2 ... SN>] [B <S1 S2 ... SN>]\n");
    printf("Attention a faire en sorte que SN < SN+1.\n");
    return 1;
  }

  ImagePPM image; image.load(argv[1]);

  vector<int> R_seuils, G_seuils, B_seuils;

  int i = 3;

  while (i < argc) {

      if (argv[i][0] == 'R') {
        i++;
        i = add_seuils(argc, argv, i, R_seuils);
      }
      else if (argv[i][0] == 'G') {
        i++;
        i = add_seuils(argc, argv, i, G_seuils);
      }
      else if (argv[i][0] == 'B') {
        i++;
        i = add_seuils(argc, argv, i, B_seuils);
      }
      else {
        cerr << "Erreur : sur le format d'entrée des données\n";
        return 1;
      }
  }

  cout << "R_seuils : "; print_v(R_seuils);
  cout << "G_seuils : "; print_v(G_seuils);
  cout << "B_seuils : "; print_v(B_seuils);

  ImagePGM R = image.get_plan(ImagePPM::PLAN_R);
  ImagePGM G = image.get_plan(ImagePPM::PLAN_G);
  ImagePGM B = image.get_plan(ImagePPM::PLAN_B);

  seuillage(R, R_seuils);
  seuillage(G, G_seuils);
  seuillage(B, B_seuils);

  image.set_plan(ImagePPM::PLAN_R, R);
  image.set_plan(ImagePPM::PLAN_G, G);
  image.set_plan(ImagePPM::PLAN_B, B);

  image.save(argv[2]);
  
  return 0;
}
