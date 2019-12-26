#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include "ImageBase.h"

using namespace std;

int main(int argc, char **argv) {

  if (argc != 2) {
    cerr << "Usage : <STRING:image.ppm>\n";
    return 1;
  }

  ImagePPM image;
  image.load(argv[1]);

  map<int, array<int, 3> > RGB_occurrences;

  ImagePGM R = image.get_plan(ImagePPM::PLAN_R);
  ImagePGM G = image.get_plan(ImagePPM::PLAN_G);
  ImagePGM B = image.get_plan(ImagePPM::PLAN_B);

  for (int i = 0; i < image.height(); ++i){
    for (int j = 0; j < image.width(); ++j){
      RGB_occurrences[R[i][j]][0]++;
      RGB_occurrences[G[i][j]][1]++;
      RGB_occurrences[B[i][j]][2]++;
    }
  }

  double nb_pixels = image.height() * image.width();

  for (auto it = RGB_occurrences.begin() ; it != RGB_occurrences.end() ; ++it){
    cout << it->first << " " << it->second[0] / nb_pixels << " " << it->second[1] / nb_pixels << " " << it->second[2] / nb_pixels << "\n";
  }

  return 0;
}
